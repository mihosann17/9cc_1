ステップ6まで進んだ。

2023/10月でステップ12とか13まで進めてた記憶もありスムーズに進んだが、
前に書いた時より今の方が色々と学びがあるかも。経験ってすごいな。

typedef struct XXX XXXという書き方は「struct XXX」を「XXX」と呼ぶことにするね。という意味

consume、expectなどでメソッド抽出してるおかげで字句解析の知識が構文解析に染み出さないようになっている。

返り値が構造体の関数は値を返せないので（必ず？）関数ポインタになる。

↑は誤り。ポインタを返り値にする関数というだけで関数ポインタではない。

returnつけ忘れてもコンパイラに怒られなくてびっくりした。怒れよそれは。
実行時エラーではなくコンパイルエラーにする方法はあるんだろうか？
というかC#とかはどうやってんだろうが気になる。

出力をprintfでやってるのでデバッグ時にprintfデバッグがめんどくさい。

テストの自動化をサクッとやれるのはよい。

雰囲気でstatic化している。現状1ファイルなので意味ないが、ファイル分ける時に効くかもと思っている。

C言語くん上から下に呼んでいくせいで循環再帰する関数作るときは少なくとも一つは関数の宣言書いてあげないといけないのだるいね。

意味解析(codegenerate部分)の最適化とかもやる？

関数名小文字だと変な気持ち。グローバル変数も変な気持ち。
