# OpenSiv3D-OCR
OpenSiv3Dで文字認識を行います。
詳細は[こちら](https://qiita.com/polyester-CTRL/items/82f6fb2c1319c772814f)をご覧ください。
## ライブラリ
- [OpenSiv3D](https://github.com/Siv3D/OpenSiv3D) (0.6)
- [Tesseract OCR](https://github.com/tesseract-ocr/tesseract) (4.1.1)

## 環境構築の方法(Windows)

### OpenSiv3Dのインストール
1. [この記事](https://zenn.dev/reputeless/articles/article-build-siv3d)に従ってOpenSiv3Dをビルドします。

### Tesseractのインストール
1. Visual StudioインストーラーからVisual Studioに英語の言語パックを追加します。（追加していない場合）
1. Tesseractはvcpkgからインストールすることができます。まず[vcpkg](https://github.com/microsoft/vcpkg)をGitHubからダウンロードし、中にある「bootstrap-vcpkg.bat」を実行します。
2. vcpkgのある場所で管理者権限でコマンドプロンプトを開き`vcpkg install tesseract:x64-windows-static`というコマンドを実行します。
3. しばらく待つとインストールが完了します。

### Visual Studioの設定
1. OpenSiv3D.slnのあるフォルダの中にtesseractlibというフォルダを作ります。
2. tesseractlibの中にdebug, releaseフォルダを作ります。
3. debugの中にC:\ ...\vcpkg\installed\x64-windows-static\debug\libの中身をコピーします。
4. releaseの中にC:\ ...\vcpkg\installed\x64-windows-static\libの中身をコピーします。
5. Visual Studioのインクルードディレクトリに$(SolutionDir)、C:\ ... \vcpkg\installed\x64-windows-static\include、およびC:\ ... \OpenSiv3D_0_6_master\Siv3D\src\ThirdParty-prebuilt\　を加えます。
6. Visual StudioのライブラリディレクトリにC:\ ... \vcpkg\installed\x64-windows-static\debug\libを加えます。

### 言語データの用意
1. https://github.com/tesseract-ocr/tessdata_fast からjpn.traineddata（縦書きの文章を認識したい場合はjpn_vert.traineddata）をダウンロードします。ダウンロードしたファイルは、`App` フォルダに新しくフォルダを作り、その中に置いてください。(ここでは`data`というフォルダを作ったものとします。）
