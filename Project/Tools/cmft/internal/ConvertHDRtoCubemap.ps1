# 入力ファイル（ドラッグ＆ドロップされたファイル）
param (
    [string]$inputFile
)

# スクリプトのある場所を基準にパス取得
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$cmftPath = Join-Path $scriptDir "cmftRelease.exe"

# 出力フォルダを指定（なければ作成）
$outputDir = Join-Path $scriptDir "../../../Assets/Images/"
if (-not (Test-Path $outputDir)) {
    New-Item -ItemType Directory -Path $outputDir | Out-Null
}

# 入力ファイルがなければ終了
if (-not (Test-Path $inputFile)) {
    Write-Host "File not found: $inputFile"
    pause
    exit
}

# 出力ファイル名（拡張子なし）
$filename = [System.IO.Path]::GetFileNameWithoutExtension($inputFile)

# 出力ファイルのフルパス
$outputFilePath = Join-Path $outputDir $filename

# cmft コマンド組み立て
$cmd = "& `"$cmftPath`" --input `"$inputFile`" --filter radianceFilterD3d --outputNum 1 --output0 `"$outputFilePath`" --output0params dds,bgra8,cubemap --dstFaceSize 1024"

# 実行
Invoke-Expression $cmd

Write-Host "Converted done: $outputFilePath.dds"
pause