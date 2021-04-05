// 参考にしたコード
// https://github.com/opencv/opencv_contrib/blob/master/modules/text/src/ocr_tesseract.cpp#L206
// This software includes the work that is distributed in the Apache License 2.0
// http://www.apache.org/licenses/LICENSE-2.0



# define NO_S3D_USING
# include <Siv3D.hpp> // OpenSiv3D v0.6
# include <Siv3D/OpenCV_Bridge.hpp>
# include <tesseract/baseapi.h>
# include <leptonica/allheaders.h>
# include <tesseractlib.hpp>



SIV3D_SET(s3d::EngineOption::Renderer::Direct3D11)
//SIV3D_SET(EngineOption::Renderer::OpenGL)
//SIV3D_SET(EngineOption::D3D11Driver::WARP)


struct Result
{
  s3d::String word;
  s3d::Rect box; 
  double probability = 0.0;
};

s3d::Array<Result> UseTesseractAPI(s3d::Image handwritten)
{
  s3d::Array<Result> results;
  tesseract::TessBaseAPI api; 
  if (api.Init("tessdata", "jpn"))
  {
    s3d::Print << U"Tesseract error";
    return results;
  }

  cv::Mat cvImage = s3d::OpenCV_Bridge::ToMatVec3bBGR(handwritten);
  
  // ここから
  // https://github.com/opencv/opencv_contrib/blob/master/modules/text/src/ocr_tesseract.cpp#L206
  api.SetImage((uchar*)cvImage.data, cvImage.size().width, cvImage.size().height, cvImage.channels(), cvImage.step1());
  api.Recognize(0);

  tesseract::ResultIterator* ri = api.GetIterator();
  tesseract::PageIteratorLevel level = tesseract::RIL_WORD;

  if (ri != 0)
  {
    do 
    {
      Result res;

      // 信頼度
      res.probability = ri->Confidence(level);
      // 検出した文字の場所
      int x1, y1, x2, y2;
      ri->BoundingBox(level, &x1, &y1, &x2, &y2);
      res.box = s3d::Rect(x1, y1, x2 - x1, y2 - y1);
      


      // 文字列をs3d::Stringに変換
      res.word = s3d::Unicode::FromUTF8(ri->GetUTF8Text(level));
      /*
      if (res.word == NULL)
      {
        continue;
      }
      */
      


      results.push_back(res);
    } while (ri->Next(level));
    delete ri;
  }

  // ここまで


  api.Clear();

  return results;
}


void Main()
{

  s3d::Scene::SetBackground(s3d::Palette::Black);
  s3d::Scene::SetResizeMode(s3d::ResizeMode::Keep);
  s3d::Window::SetStyle(s3d::WindowStyle::Sizable);

  // 認識結果を保存する
  s3d::Array<s3d::Rect> results;
  s3d::AsyncTask<s3d::Array<Result>> task;

  // 画像読み込み
  const s3d::Image handwritten(U"Screenshot/hand.png");

  


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
      task = s3d::CreateAsyncTask(UseTesseractAPI, s3d::Image(dynamicImage));
    }

    // マルチスレッド処理が終了したら
    if (task.isReady())
    {
      auto returnValue = task.get();

      
      for (const auto &result : returnValue)
      {
        results.push_back(result.box);
        s3d::Print << result.word;
      }
    }
    // テクスチャを表示
    texture.draw();

    // 文字の場所を長方形で示す
    for (const auto& result : results) 
    {
      result.drawFrame(1, 1, s3d::Palette::Orange);
    }
  }
}
