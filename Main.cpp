// 参考にしたコード
// https://github.com/opencv/opencv_contrib/blob/master/modules/text/src/ocr_tesseract.cpp#L206
// This software includes the work that is distributed in the Apache License 2.0
// http://www.apache.org/licenses/LICENSE-2.0



# define NO_S3D_USING
# include <Siv3D.hpp> // OpenSiv3D v0.6
# include <Siv3D/OpenCV_Bridge.hpp>
# include <opencv2/opencv.hpp>
# include <opencv2/text.hpp>
# include <tesseract/baseapi.h>
# include <leptonica/allheaders.h>




SIV3D_SET(s3d::EngineOption::Renderer::Direct3D11)
//SIV3D_SET(EngineOption::Renderer::OpenGL)
//SIV3D_SET(EngineOption::D3D11Driver::WARP)

s3d::String UseTesseractAPI(s3d::Image handwritten)
{
  s3d::String s;
  char* outText;
  tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
  if (api->Init("tessdata", "jpn"))
  {
    s3d::Print << U"Tesseract error";
    return U"Tesseract error";
  }

  cv::Mat cvImage = s3d::OpenCV_Bridge::ToMatVec3bBGR(handwritten);
  
  // ここから
  // https://github.com/opencv/opencv_contrib/blob/master/modules/text/src/ocr_tesseract.cpp#L206
  api->SetImage((uchar*)cvImage.data, cvImage.size().width, cvImage.size().height, cvImage.channels(), cvImage.step1());

  // ここまで
  

  outText = api->GetUTF8Text();


  api->End();
  delete api;

  s = s3d::Unicode::FromUTF8(outText);
  return s;
}

/*
s3d::String RecognizeCharacterFromImage(s3d::Image handwritten,
  std::string& text,
  std::vector<cv::Rect>& boxes,
  std::vector<std::string>& words,
  std::vector<float>& confidences)
{
  
  // 画像変換
  cv::Mat image = s3d::OpenCV_Bridge::ToMatVec3bBGR(handwritten);

  // グレースケール化
  cv::Mat gray;
  cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);

  {
    // 文字認識クラスのインスタンス生成
    cv::Ptr<cv::text::OCRTesseract> ocr = cv::text::OCRTesseract::create("tessdata", "jpn");
    // ホワイトリストを消す
    ocr->setWhiteList("");

    // 文字認識の実行
    ocr->run(gray, text, &boxes, &words, &confidences);
  }
  // 結果出力
  s3d::String s = U"error";
  if (boxes.size() > 0)
  {
    // 文字コードを変換
    s = s3d::Unicode::FromUTF8(text);
  }

  return s;
}
*/



void Main()
{

  s3d::Scene::SetBackground(s3d::Palette::Black);
  s3d::Scene::SetResizeMode(s3d::ResizeMode::Keep);
  s3d::Window::SetStyle(s3d::WindowStyle::Sizable);

  // 認識結果を保存する
  std::string text;
  std::vector<cv::Rect> boxes;
  std::vector<std::string> words;
  std::vector<float> confidences;
  s3d::Array<s3d::Rect> results;
  
  
  s3d::AsyncTask<s3d::String> task;

  // 画像読み込み
  const s3d::Image handwritten(U"Screenshot/hand.png");

  // useTesseract 
  s3d::String res = UseTesseractAPI(handwritten);
  s3d::Print << res;


  // スケッチから文字認識

   // キャンバスのサイズ
  constexpr s3d::Size canvasSize(600, 600);

  // ペンの太さ
  constexpr s3d::int32 thickness = 8;

  // ペンの色
  constexpr s3d::Color penColor = s3d::Palette::Orange;

  // 書き込み用の画像データを用意
  s3d::Image dynamicImage(canvasSize, s3d::Palette::White);

  // 表示用のテクスチャ（内容を更新するので DynamicTexture）
  s3d::DynamicTexture texture(dynamicImage);


  while (s3d::System::Update())
  {
    if (s3d::MouseL.pressed())
    {
      // 書き込む線の始点は直前のフレームのマウスカーソル座標
      // （初回はタッチ操作時の座標のジャンプを防ぐため、現在のマウスカーソル座標にする）
      const s3d::Point from = s3d::MouseL.down() ? s3d::Cursor::Pos() : s3d::Cursor::PreviousPos();

      // 書き込む線の終点は現在のマウスカーソル座標
      const s3d::Point to = s3d::Cursor::Pos();

      // image に線を書き込む
      s3d::Line(from, to).overwrite(dynamicImage, thickness, penColor);

      // 書き込み終わった image でテクスチャを更新
      texture.fill(dynamicImage);
    }

    // 描いたものを消去するボタンが押されたら
    if (s3d::SimpleGUI::Button(U"Clear", s3d::Vec2(640, 40), 120))
    {
      // 画像を白で塗りつぶす
      dynamicImage.fill(s3d::Palette::White);

      // 塗りつぶし終わった image でテクスチャを更新
      texture.fill(dynamicImage);

      // 認識した場所の情報を消す
      results.clear();
    }

    // 認識を開始するボタンが押されたら
    if (s3d::SimpleGUI::Button(U"Recognize", s3d::Vec2(640, 100), 120))
    {
      // マルチスレッドで認識する関数を呼ぶ
      // task = CreateAsyncTask(RecognizeCharacterFromImage, std::ref(dynamicImage), std::ref(text), std::ref(boxes), std::ref(words), std::ref(confidences));
    }

    // マルチスレッド処理が終了したら
    if (task.isReady())
    {
      // 認識結果を表示
      s3d::Print << task.get();

      // 文字の場所を長方形で示す
      for (int i = 0; i < boxes.size(); i++)
      {
        s3d::Rect(boxes[i].x, boxes[i].y, boxes[i].width, boxes[i].height).drawFrame(1, 1, s3d::Palette::Orange);
        results.push_back(s3d::Rect(boxes[i].x, boxes[i].y, boxes[i].width, boxes[i].height));
      }
    }
    // テクスチャを表示
    texture.draw();
    for (const auto& result : results) 
    {
      result.drawFrame(1, 1, s3d::Palette::Orange);
    }
  }
}
