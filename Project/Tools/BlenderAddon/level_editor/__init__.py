import bpy
import mathutils
import bpy_extras
import gpu
import gpu_extras.batch
import json
import copy
from mathutils import Vector


bl_info = {
    "name": "レベルエディタ",
    "author": "Kazuki Ihara",
    "version": (1, 1, 0),
    "blender": (4, 4, 0),
    "location": "",
    "description": "シーンを書き出す簡易レベルエディタ",
    "warning": "",
    "category": "Object"
}


identifier_items = [
    ('CAMERA_EYE',    'CameraEyeControlPoint',    'カメラの Eye'),
    ('CAMERA_TARGET', 'CameraTargetControlPoint', 'カメラの Target'),
]


def register():
    # === Object プロパティ ===
    bpy.types.Object.empty_identifier = bpy.props.EnumProperty(
        name="Identifier", description="EMPTY の識別子",
        items=identifier_items, default='CAMERA_EYE',
    )
    bpy.types.Object.cp_order = bpy.props.IntProperty(
        name="Order", description="コントロールポイントの並び順 (0,1,2…)",
        default=0, min=0,
    )
    bpy.types.Object.cp_camera = bpy.props.StringProperty(
        name="Camera", description="所属カメラ名",
        default="", maxlen=64,
    )
    bpy.types.Object.mesh_visible = bpy.props.BoolProperty(
        name="Visible", description="画面に描画するかどうか",
        default=True,
    )

    for cls in classes:
        bpy.utils.register_class(cls)

    bpy.types.TOPBAR_MT_editor_menus.append(TOPBAR_MT_my_menu.submenu)

    DrawCollider.handle = bpy.types.SpaceView3D.draw_handler_add(
        DrawCollider.draw_collider, (), "WINDOW", "POST_VIEW")

    print("レベルエディタが有効化されました")


def unregister():
    # 追加プロパティ削除
    del bpy.types.Object.mesh_visible
    del bpy.types.Object.cp_camera
    del bpy.types.Object.cp_order
    del bpy.types.Object.empty_identifier

    bpy.types.TOPBAR_MT_editor_menus.remove(TOPBAR_MT_my_menu.submenu)
    bpy.types.SpaceView3D.draw_handler_remove(DrawCollider.handle, "WINDOW")

    for cls in classes:
        bpy.utils.unregister_class(cls)

    print("レベルエディタが無効化されました")


class TOPBAR_MT_my_menu(bpy.types.Menu):
    bl_idname = "TOPBAR_MT_my_manu"
    bl_label = "Mymenu"
    bl_description = "拡張メニュー by " + bl_info["author"]

    def draw(self, context):
        self.layout.operator(MYADDON_OT_export_scene.bl_idname,
                             text=MYADDON_OT_export_scene.bl_label)

    def submenu(self, _context):
        self.layout.menu(TOPBAR_MT_my_menu.bl_idname)

class MYADDON_OT_export_scene(bpy.types.Operator, bpy_extras.io_utils.ExportHelper):
    bl_idname = "myaddon.myaddon_ot_export_scene"
    bl_label = "シーン出力"
    filename_ext = ".json"

    def execute(self, _context):
        self.export_json()
        self.report({'INFO'}, "シーン情報を Export しました")
        return {'FINISHED'}

    # JSON エクスポート本体
    def export_json(self):
        root = {"name": "scene", "objects": []}

        for obj in bpy.context.scene.objects:
            if obj.parent:
                continue
            self.pack_object_recursive(root["objects"], obj)

        text = json.dumps(root, ensure_ascii=False, indent=4)
        print(text)
        with open(self.filepath, "wt", encoding="utf-8") as f:
            f.write(text)

    # 再帰でオブジェクトをパック
    def pack_object_recursive(self, dst_list, obj, level=0):
        jd = {"type": obj.type, "name": obj.name}

        if obj.type == "CAMERA":
            loc = obj.matrix_local.to_translation()
            eye = (loc.x, loc.z, loc.y)

            forward = (obj.matrix_local.to_3x3()
                       @ Vector((0.0, 0.0, -1.0))).normalized()
            tgt = loc + forward
            target = (tgt.x, tgt.z, tgt.y)

            cd = obj.data
            jd["camera_data"] = {
                "target": target,
                "eye": eye,
                "fovY": cd.angle_y,
                "nearClip": cd.clip_start,
                "farClip": cd.clip_end
            }

        else:
            trans, rot_q, scale = obj.matrix_local.decompose()
            rot_q_xyzw = (-rot_q.x, -rot_q.z, -rot_q.y, rot_q.w)
            jd["transform"] = {
                "translate": (trans.x, trans.z, trans.y),
                "rotate": rot_q_xyzw,
                "scale": (scale.x, scale.z, scale.y)
            }

        # 追加カスタムプロパティ類 ---------------
        if "model_name" in obj:
            jd["model_name"] = obj["model_name"]
        if obj.type == "MESH":
            jd["visible"] = bool(obj.mesh_visible)

        # EMPTY 用識別子
        if obj.type == "EMPTY" and obj.empty_identifier:
            jd["identifier"] = obj.empty_identifier
            jd["order"] = int(obj.cp_order)
            if obj.cp_camera:
                jd["camera_name"] = obj.cp_camera

        # collider
        if "collider" in obj:
            jd["collider"] = {
                "type": obj["collider"],
                "center": obj["collider_center"].to_list(),
                "size": obj["collider_size"].to_list()
            }

        dst_list.append(jd)

        # 子も再帰
        if obj.children:
            jd["children"] = []
            for ch in obj.children:
                self.pack_object_recursive(jd["children"], ch, level + 1)

# ModelName Visible
class OBJECT_PT_model_name(bpy.types.Panel):
    bl_idname = "OBJECT_PT_model_name"
    bl_label = "ModelName"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    @classmethod
    def poll(cls, context):
        return context.object and context.object.type == 'MESH'

    def draw(self, context):
        obj = context.object
        col = self.layout.column()
        if "model_name" in obj:
            col.prop(obj, '["model_name"]', text=self.bl_label)
        else:
            col.operator(MYADDON_OT_add_modelname.bl_idname)
        col.prop(obj, "mesh_visible", text="Visible")

class MYADDON_OT_add_modelname(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_modelname"
    bl_label = "modelName 追加"
    bl_options = {"REGISTER", "UNDO"}

    def execute(self, context):
        context.object["model_name"] = ""
        return {"FINISHED"}

# パネル: EMPTY 用
class OBJECT_PT_empty_identifier(bpy.types.Panel):
    bl_idname = "OBJECT_PT_empty_identifier"
    bl_label = "ControlPoint"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    @classmethod
    def poll(cls, context):
        return context.object and context.object.type == 'EMPTY'

    def draw(self, context):
        obj = context.object
        self.layout.prop(obj, "empty_identifier", text="Identifier")
        self.layout.prop(obj, "cp_order",        text="Order")
        self.layout.prop(obj, "cp_camera",       text="Camera")

class DrawCollider:
    handle = None

    @staticmethod
    def draw_collider():
        verts = {"pos": []}
        indices = []
        box_offsets = [(-.5,-.5,-.5), (.5,-.5,-.5), (-.5,.5,-.5), (.5,.5,-.5),
                       (-.5,-.5,.5), (.5,-.5,.5), (-.5,.5,.5), (.5,.5,.5)]

        for obj in bpy.context.scene.objects:
            if "collider" not in obj:
                continue

            center = Vector(obj["collider_center"])
            size   = Vector(obj["collider_size"])

            start = len(verts["pos"])
            for off in box_offsets:
                p = center + Vector(off) * size
                verts["pos"].append(obj.matrix_world @ p)

            # 12 lines (indices)
            lines = [(0,1),(2,3),(0,2),(1,3),
                     (4,5),(6,7),(4,6),(5,7),
                     (0,4),(1,5),(2,6),(3,7)]
            for a,b in lines:
                indices.append([start+a, start+b])

        shader = gpu.shader.from_builtin("UNIFORM_COLOR")
        batch  = gpu_extras.batch.batch_for_shader(shader, "LINES",
                                                   verts, indices=indices)
        shader.bind()
        shader.uniform_float("color", (0.5,1.0,1.0,1.0))
        batch.draw(shader)


class MYADDON_OT_add_collider(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_collider"
    bl_label = "コライダー追加"
    bl_options = {"REGISTER", "UNDO"}

    def execute(self, context):
        obj = context.object
        obj["collider"] = "Box"
        obj["collider_center"] = mathutils.Vector((0,0,0))
        obj["collider_size"]   = mathutils.Vector((2,2,2))
        return {"FINISHED"}

class OBJECT_PT_collider(bpy.types.Panel):
    bl_idname = "OBJECT_PT_collider"
    bl_label = "Collider"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    def draw(self, context):
        obj = context.object
        if "collider" in obj:
            col = self.layout.column()
            col.prop(obj, '["collider"]', text="Type")
            col.prop(obj, '["collider_center"]', text="Center")
            col.prop(obj, '["collider_size"]', text="Size")
        else:
            self.layout.operator(MYADDON_OT_add_collider.bl_idname)

# クラス登録リスト
classes = (
    MYADDON_OT_export_scene,
    TOPBAR_MT_my_menu,
    MYADDON_OT_add_modelname,
    OBJECT_PT_model_name,
    OBJECT_PT_empty_identifier,
    MYADDON_OT_add_collider,
    OBJECT_PT_collider,
)

if __name__ == "__main__":
    try:
        unregister()
    except Exception:
        pass
    register()
