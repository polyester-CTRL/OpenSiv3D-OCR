# OpenSiv3D-OCR
OpenSiv3Dで文字認識を行います。
詳細は[こちら](https://qiita.com/polyester-CTRL/items/82f6fb2c1319c772814f)をご覧ください。
## ライブラリ
- [OpenSiv3D](https://github.com/Siv3D/OpenSiv3D) (0.6)
- [OpenCV](https://github.com/opencv/opencv) (4.5.1)
- [OpenCV_contrib](https://github.com/opencv/opencv_contrib) (4.5.1)
- [Tesseract OCR](https://github.com/tesseract-ocr/tesseract) (4.1.1)

## 環境構築の方法(Windows)
1. vcpkgをインストールする。
1. コマンドプロンプトでvcpkg install tesseract:x64-windows-staticと入力する。
1. [この記事](https://zenn.dev/reputeless/articles/article-build-siv3d)に従ってOpenSiv3Dをビルド。
2. OpenSiv3D.slnのあるフォルダの中にtesseractlibというフォルダを作る。
3. tesseractlibの中にdebug, releaseフォルダを作る。
4. debugの中にC:\ ...\vcpkg\installed\x64-windows-static\debug\libの中身をコピーする。
5. Visual Studioのインクルードディレクトリに$(SolutionDir)、C:\ ... \vcpkg\installed\x64-windows-static\include、C:\ ... \OpenSiv3D_0_6_master\Siv3D\src\ThirdParty-prebuilt\　を加える。
6. Visual StudioのライブラリディレクトリにC:\ ... \vcpkg\installed\x64-windows-static\debug\libを加える。
