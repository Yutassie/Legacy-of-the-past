#include <stdio.h>
#include <stdlib.h>

FILE *fp1, *fp2;//ファイル読み込み用・書き込み用
char *fname1 = "mondai.txt", *fname2 = "kaitou.txt";//読み込み用・書き込み用ファイル名
int width = 0;//パズルの横幅(0からF）
int height = 0;//パズルの縦幅
int times, selCost, swapCost;//選択回数、選択コスト、交換コスト
int i, j;//for文用
struct masu{
	int x;
	int y;
	int status;//(-1=何もしてない、0=選択中、1=並べ替え終わり）
};

struct masu searched_tile;
struct masu moved_tile;
struct masu next_select;
int a = -1;
int moveDflag = 0;
int moveLflag = 0;
int breakflag = 0;

struct masu serched;//search関数で検索した値が見つかった座標が帰ってくる。
struct masu selected;//今選択（掴んでいる）ますの情報を入れる。
struct masu tile[16][16];//パズルのタイルのデータが入る予定
struct masu processing;//今揃えている列の値が入る（一段目なら０、二段目なら１）

struct masu Im_searched_tile;
struct masu Im_moved_tile;
struct masu Im_selected;//今選択（掴んでいる）ますの情報を入れる。
struct masu Im_tile[16][16];

char sousa[300000];//交換操作手順

int swap = 0;//ラインごとの交換回数をカウント
int sel = 0;//総選択回数をカウント

void file_input(void);//問題テキストファイルを読み込む。開けないとプログラム強制終了 
void position_output(int x, int y);//選択画像位置を回答ファイルに出力 
void swap_output(void);//交換回数・軌跡を回答ファイルに出力 
void line_clear(void);//交換回数・軌跡をリセット

int select(int x, int y); //場所（ｘ，ｙ）のタイルをつかむ失敗したら-1を返す。

int move_R(int selected_x, int selected_y); //掴んでいるタイルとその右のタイルを交換（失敗したら"１を返す）
int move_L(int selected_x, int selected_y); //掴んでいるタイルとその左のタイルを交換（失敗したら"１を返す）
int move_U(int selected_x, int selected_y); //掴んでいるタイルとその上のタイルを交換（失敗したら"１を返す）
int move_D(int selected_x, int selected_y); //掴んでいるタイルとその下のタイルを交換（失敗したら"１を返す）

struct masu search_tile(int x, int y);//（ｘ，ｙ）のタイルがどこの座標にあるか探して見つかったらその場所を返す。失敗したら-1

int tile_move(int goal_x, int goal_y);//x,y座標のタイルをgoal_x、goal_yまで移動させる。move_Rとか使って移動させる。成功したら0、失敗したら-1を...

int tile_shita_sort();//一番下の列をソートする（失敗したら-1)