import bpy
import math
import bpy_extras
import gpu
import gpu_extras.batch
import copy
import mathutils
import json

# ブレンダーに登録するアドオン情報
bl_info = {
    "name": "レベルエディタ",
    "author": "Kazuki Ihara",
    "version": (1, 0),
    "blender": (4, 4, 0),
    "location": "",
    "description": "レベルエディタ",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "category": "Object"
}

#Add-on有効化時コールバック
def register():
    #Blenderにクラスを登録
    for cls in classes:
        bpy.utils.register_class(cls)

    # メニューに項目を追加
    bpy.types.TOPBAR_MT_editor_menus.append(TOPBAR_MT_my_menu.submenu)
    #3Dビューに描画関数を追加
    DrawCollider.handle = bpy.types.SpaceView3D.draw_handler_add(DrawCollider.draw_collider, (),"WINDOW", "POST_VIEW")
    print("レベルエディタが有効化されました")

#Add-on無効化時コールバック
def unregister():
    # メニューから項目を削除
    bpy.types.TOPBAR_MT_editor_menus.remove(TOPBAR_MT_my_menu.submenu)
    #3Dビューから描画関数を削除
    bpy.types.SpaceView3D.draw_handler_remove(DrawCollider.handle, "WINDOW")
    
    #Blenderにクラスを登録
    for cls in classes:
        bpy.utils.unregister_class(cls)
    print("レベルエディタが無効化されました")

# テスト実行コード
if __name__ == "__main__":
    register()

# メニュー項目描画
def draw_menu_manual(self,context):  
    #self: 呼び出し元のクラスインスタンスC++だとThisPtr
    #Context: カーソルを合わせたときのポップアップのカスタマイズなどに使用
    
    #トップバーの[エディタメニュー]に項目(オペレータ)を追加
    self.layout.operator("wm.url_open_preset",text="Manual",icon='HELP')

#トップバーの拡張メニュー
class TOPBAR_MT_my_menu(bpy.types.Menu):
    #Blenderがクラスを識別するための固有の文字列
    bl_idname = "TOPBAR_MT_my_manu"
    #メニューのラベルとして表示される文字列
    bl_label = "Mymenu"
    #著者表示用の文字列
    bl_description = "拡張メニュー by" + bl_info["author"]

    #サブメニューの描画
    def draw(self, context):
        #トップバーの[エディタメニュー]に項目(オペレーター)を追加
        self.layout.operator(MYADDON_OT_stretch_vertex.bl_idname,
            text = MYADDON_OT_stretch_vertex.bl_label)
        
        #ICO球生成項目を追加
        self.layout.operator(MYADDON_OT_create_ico_sphere.bl_idname,
            text = MYADDON_OT_create_ico_sphere.bl_label)

        #シーン出力
        self.layout.operator(MYADDON_OT_export_scene.bl_idname,
            text = MYADDON_OT_export_scene.bl_label)




    #既存のメニューにサブメニューを追加
    def submenu(self, context):

        #ID指定でサブメニューを追加
        self.layout.menu(TOPBAR_MT_my_menu.bl_idname)

class MYADDON_OT_stretch_vertex(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_stretch_vertex"
    bl_label = "頂点を伸ばす"
    bl_description = "頂点座標を引っ張って伸ばす"
    #リドゥ、アンドゥ可能オプション
    bl_options = {'REGISTER','UNDO'}

    #メニューを実行したときに呼ばれるコールバック関数
    def execute(self,context):
        bpy.data.objects["Cube"].data.vertices[0].co.x += 1.0
        print("頂点を伸ばしました。")

        #オペレータの命令終了を通知
        return{'FINISHED'}

#オペレータ　ICO球生成
class MYADDON_OT_create_ico_sphere(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_create_object"
    bl_label = "ICO球生成"
    bl_description = "ICO球を生成します"
    bl_options ={'REGISTER','UNDO'}

    #メニューを実行したときに呼ばれる関数
    def execute(self,context):
        bpy.ops.mesh.primitive_ico_sphere_add()
        print("ICO球を生成しました。")

        return {'FINISHED'}
    
#オペレータ　シーン出力
class MYADDON_OT_export_scene(bpy.types.Operator,bpy_extras.io_utils.ExportHelper):
    bl_idname = "myaddon.myaddon_ot_export_scene"
    bl_label = "シーン出力"
    bl_description = "シーン情報をExportします"
    #出力するファイルの拡張子
    filename_ext = ".json"

    def write_and_print(self,file,str):
        print(str)

        file.write(str)
        file.write("\n")

    def execute(self,context):

        print("シーン情報をExportします")
        #ファイルに出力
        self.export_json()

        print("シーン情報をExportしました")
        self.report({'INFO'},"シーン情報をExportしました")

        return {'FINISHED'}

    def export(self):
        """ファイルに出力"""

        print("シーン情報出力開始... %r" % self.filepath)

        #ファイルをテキスト形式で書き出し用にオープン
        #スコープを抜けると自動的にクローズされる
        with open(self.filepath, "wt") as file:

            #ファイルに文字列を書き込む
            file.write("SCENE\n")
        
            #シーン内の全オブジェクトについて
            for object in bpy.context.scene.objects:
                
                #親オブジェクトがあるものはスキップ
                if(object.parent):
                    continue

                #シーン直下のオブジェクトをルートノード(深さ0)とし、再帰関数で走査
                self.parse_scene_recursive(file, object,0)
    
    def export_json(self):
        """JSON形式でファイルに出力"""
        
        #保存する情報をまとめるdict
        json_object_root = dict()

        #ノード名
        json_object_root["name"] = "scene"
        #オブジェクトリストを作成
        json_object_root["objects"] = list()

        #シーン内の全オブジェクトを走査してパック
        for object in bpy.context.scene.objects:

            #親オブジェクトがあるものはスキップ
            if(object.parent):
                continue

            #シーン直下のオブジェクトをルートノード(深さ0)とし、再帰関数で走査
            self.parse_scene_recursive_json(json_object_root["objects"],object, 0)


        #オブジェクトをJSON文字列にエンコード
        json_text = json.dumps(json_object_root, ensure_ascii=False, cls=json.JSONEncoder, indent=4)
        #コンソールに表示してみる
        print(json_text)

        #ファイルをテキスト形式で書き出し用にオープン
        #スコープを抜けると自動的にクローズされる
        with open(self.filepath, "wt", encoding = "utf-8") as file:
            
            #ファイルに文字列を書き込む
            file.write(json_text)

    def parse_scene_recursive_json(self,data_parent,object,level):

        #シーンのオブジェクト一個分のjsonオブジェクト生成
        json_object = dict()
        #オブジェクト種類
        json_object["type"] = object.type
        #オブジェクト名
        json_object["name"] = object.name

        #その他情報をパック
        #オブジェクトのローカルトランスフォームから
        #平行移動、回転、スケールを抽出
        trans, rot_q, scale = object.matrix_local.decompose()
        rot_q_xyzw = (-rot_q.x,-rot_q.z,-rot_q.y,rot_q.w)   # ← Blender は wxyz なので並べ替え

        transform = {
            "translate": (trans.x, trans.z, trans.y), # 座標系ここで変換
            "rotate": rot_q_xyzw,                
            "scale": (scale.x, scale.z,scale.y)
        }   

        #まとめて1個分のjsonオブジェクトに登録
        json_object["transform"] = transform

        #カスタムプロパティ'file name'
        if "model_name" in object:
            json_object["model_name"] = object["model_name"]

        #カスタムプロパティ'collider'
        if "collider" in object:
            collider = dict()
            collider["type"] =object["collider"]
            collider["center"] =object["collider_center"].to_list()
            collider["size"] =object["collider_size"].to_list()
            json_object["collider"] =collider

        #1個分のjsonオブジェクトを親オブジェクトに登録
        data_parent.append(json_object)

        #直接の子供リストを走査
        if len(object.children) >0:
            #子ノードリストを作成
            json_object["children"] = list()

            #子ノードへ進む(深さが1上がる)
            for child in object.children:
                self.parse_scene_recursive_json(json_object["children"],child,level +1)


#パネル　ファイル名
class OBJECT_PT_model_name(bpy.types.Panel):
    """オブジェクトのモデルネームパネル"""
    bl_idname = "OBJECT_PT_model_name"
    bl_label = "ModelName"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    #サブメニューの描画
    def draw(self,context):

        #パネルに項目を追加
        if "model_name" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object,'["model_name"]', text = self.bl_label)
        else:
            #プロパティがなければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_modelname.bl_idname)

#オペレータ　カスタムプロパティ
class MYADDON_OT_add_modelname(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_modelname"
    bl_label = "modelName 追加"
    bl_description = "['model_name']カスタムプロパティを追加します"
    bl_options = {"REGISTER", "UNDO"}

    def execute(self,context):

        #['model_name']カスタムプロパティを追加
        context.object["model_name"] = ""

        return {"FINISHED"}

#コライダー描画
class DrawCollider:

    #描画ハンドル
    handle = None

    #3Dビューに登録する描画関数
    def draw_collider():
        #頂点データ
        vertices = {"pos":[]}
        #インデックスデータ
        indices = []
        
        #各頂点と、オブジェクト中心からのオフセット
        offsets = [
                    [-0.5,-0.5,-0.5],
                    [+0.5,-0.5,-0.5],
                    [-0.5,+0.5,-0.5],
                    [+0.5,+0.5,-0.5],
                    [-0.5,-0.5,+0.5],
                    [+0.5,-0.5,+0.5],
                    [-0.5,+0.5,+0.5],
                    [+0.5,+0.5,+0.5],
        ]
        #立方体のX,Y,Z方向サイズ
        size = [2,2,2]

        #現在シーンのオブジェクトリストを走査
        for object in bpy.context.scene.objects:

            #コライダープロパティがなければ、描画をスキップ
            if not "collider" in object:
                continue

            #中心点、サイズの変数を宣言
            center = mathutils.Vector((0,0,0))
            size = mathutils.Vector((2,2,2))

            #プロパティから値を取得
            center[0]=object["collider_center"][0]
            center[1]=object["collider_center"][1]
            center[2]=object["collider_center"][2]
            size[0]=object["collider_size"][0]
            size[1]=object["collider_size"][1]
            size[2]=object["collider_size"][2]

            #追加前の頂点数
            start = len(vertices["pos"])
            #BoxのB頂点分回す
            for offset in offsets:
                #オブジェクトの中心座標をコピー
                pos = copy.copy(center)
                #中心点を基準に各頂点ごとにずらす
                pos[0]+=offset[0]*size[0]
                pos[1]+=offset[1]*size[1]
                pos[2]+=offset[2]*size[2]
                #ローカル座標からワールド座標に変換
                pos = object.matrix_world @ pos
                #頂点データリストに座標を追加
                vertices['pos'].append(pos)

                #前面を構成する辺の頂点インデックス
                indices.append([start+0,start+1])
                indices.append([start+2,start+3])
                indices.append([start+0,start+2])
                indices.append([start+1,start+3])
                #奥面を構成する辺の頂点インデックス
                indices.append([start+4,start+5])
                indices.append([start+6,start+7])
                indices.append([start+4,start+6])
                indices.append([start+5,start+7])
                #前と頂点をつなぐ辺の頂点インデックス
                indices.append([start+0,start+4])
                indices.append([start+1,start+5])
                indices.append([start+2,start+6])
                indices.append([start+3,start+7])

        #ビルドインのシェーダーを取得
        shader = gpu.shader.from_builtin("UNIFORM_COLOR")

        #バッチを作成
        batch = gpu_extras.batch.batch_for_shader(shader, "LINES", vertices, indices = indices)

        #シェーダーのパラメータ設定
        color = [0.5,1.0,1.0,1.0]
        shader.bind()
        shader.uniform_float("color", color)
        #描画
        batch.draw(shader)

#オペレータカスタムプロパティ追加
class MYADDON_OT_add_collider(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_collider"
    bl_label = "コライダー追加"
    bl_description = "['collider']カスタムプロパティを追加します"
    bl_options = {"REGISTER", "UNDO"}

    def execute(self,context):

        #カスタムプロパティを追加
        context.object["collider"] = "Box"
        context.object["collider_center"] = mathutils.Vector((0,0,0))
        context.object["collider_size"] = mathutils.Vector((2,2,2))

        return {"FINISHED"}

#パネルコライダー
class OBJECT_PT_collider(bpy.types.Panel):
    bl_idname = "OBJECT_PT_collider"
    bl_label = "collider"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    #サブメニューの描画
    def draw(self, context):

        #パネルに項目を追加
        if "collider" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object, '["collider"]', text = "Type")
            self.layout.prop(context.object, '["collider_center"]', text = "Center")
            self.layout.prop(context.object, '["collider_size"]', text = "Size")
        else:
            #プロパティがなければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_collider.bl_idname)



#Blenderに登録するクラスリスト
classes =(
    MYADDON_OT_export_scene,
    MYADDON_OT_create_ico_sphere,
    MYADDON_OT_stretch_vertex, 
    TOPBAR_MT_my_menu,
    MYADDON_OT_add_modelname,
    OBJECT_PT_model_name,
    MYADDON_OT_add_collider,
    OBJECT_PT_collider
)