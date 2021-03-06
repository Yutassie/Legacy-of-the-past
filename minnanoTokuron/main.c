#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "FUNCTION_LIST.h"
/*///////////////////////////////////////////////////////
実行前にヘッダの更新を・・・
16*16まで選択回数2回（たまに4回）で完成させられます。
交換回数が莫大になったのでsousaの添え字は3万位にしないとわけのわからない数字列が並びます。
Im付きの関数はファイル出力をしないだけでImなしのものと処理は変わりません。もっとうまい書き方があったのかもしれませんが・・・
Imを利用すれば本来のパズルを崩すことなくいろいろな揃えるパターンを検索して最速のものを取り上げることが出来ます。
もし時間があればそんなことをしてみるのもいいかも・・・
アルゴリズムとしては
      1.下2行を残して選択一回で揃える
	  2.最後の4マスについてクルクル回して揃えることが出来るタイルを検索し選択
	  3.下2行を縦に揃えていく。この時まず最後の4マスを残しておく。
	  4.最後にクルクルするとそろう。
*////////////////////////////////////////////////////////
int main(void){


	int i, j;
	int Im_i, Im_j;
	struct masu goal_tile;

	width = 8;
	height = 6;

	//////////////////////////////////////////
	//ファイルから？乱数から？

	file_input();
	//set_masu(width, height);

	//////////////////////////////////////////

	//出力ファイルの初期化
	fp2 = fopen(fname2, "w");
	if (fp2 == NULL){
		printf("%s出力ファイルが開けません\n", fname2);
		exit(-1);
	}
	fprintf(fp2, "\n");//選択画像位置
	fclose(fp2);

	next_select.x = next_select.y = -1;

	for (j = 0; j<height; j++){
		for (i = 0; i<width; i++){
			printf("%d%d ", tile[i][j].x, tile[i][j].y);
		}
		putchar('\n');
	}

	printf("\n");

	searched_tile = search_tile(width - 1, height - 3);
	tile[searched_tile.x][searched_tile.y].status = 2;
	select(searched_tile.x, searched_tile.y);
	for (i = 0; i < height - 2; i++){
		goal_tile.y = i;
		for (j = 0; j < width; j++){
			goal_tile.x = j;
			tile_move(goal_tile.x, goal_tile.y);
			//パズルを表示
			for (int l = 0; l<height; l++){
				for (int m = 0; m<width; m++){
					printf("%d%d ", tile[m][l].x, tile[m][l].y);
				}
				putchar('\n');
			}
			putchar('\n');
		}
	}
	swap_output();
	
	//一番下の列をソート
	//洗濯一回で揃えられるパターソを探す。
	for (Im_i = width - 2; Im_i < width; Im_i++){
		for (Im_j = height - 2; Im_j < height; Im_j++){
			//Im_tileを初期化
			for (int l = 0; l<height; l++){
				for (int m = 0; m<width; m++){
					Im_tile[m][l] = tile[m][l];
				}
			}

			searched_tile = Im_search_tile(Im_i, Im_j);
			Im_tile[searched_tile.x][searched_tile.y].status = 2;
			Im_select(searched_tile.x, searched_tile.y);

			for (int f = 0; f < width - 2; f++){
				for (int p = height - 2; p < height; p++){
					goal_tile.x = f;
					goal_tile.y = p;
					Im_tile_shita2gyo_sort(goal_tile.x, goal_tile.y);
					//パズルを表示
					/*printf("Im\n");
					for (int l = 0; l<height; l++){
						for (int m = 0; m<width; m++){
							printf("%d%d ", Im_tile[m][l].x, Im_tile[m][l].y);
						}
						putchar('\n');
					}*/
				}
			}
			if (Im_tile_curucuru()){
				breakflag = 1;
			}
			if (breakflag == 1){
				break;
			}
			if (Im_j == height - 1){
				break;
			}
		}
		if (breakflag == 1){
			break;
		}
		if (Im_i == width - 1){
			break;
		}
	}
	searched_tile = search_tile(Im_i, Im_j);
	tile[searched_tile.x][searched_tile.y].status = 2;
	select(searched_tile.x, searched_tile.y);
	//一番下の列をソート
	for (i = 0; i < width - 2; i++){
		goal_tile.x = i;
		for (j = height - 2; j < height; j++){
			goal_tile.y = j;
			tile_shita2gyo_sort(goal_tile.x, goal_tile.y);
			//パズルを表示
			/*for (int l = 0; l<height; l++){
				for (int m = 0; m<width; m++){
					printf("%d%d ", tile[m][l].x, tile[m][l].y);
				}
				putchar('\n');
			}*/
		}
	}
	if (breakflag == 1)
		tile_curucuru();
	else{
		select(width - 1, height - 1);
		move_U(selected.x, selected.y);
		swap_output();
		searched_tile = search_tile(Im_i, Im_j);
		select(searched_tile.x, searched_tile.y);
		tile_curucuru();
	}
	swap_output();

	//結果を表示するよ！！
	printf("完成しました！*********************************************************\n");
	for (j = 0; j<height; j++){
		for (i = 0; i<width; i++){
			printf("%d%d ", tile[i][j].x, tile[i][j].y);
		}
		putchar('\n');
	}

	printf("選択回数：%d", sel);


	getchar();
	return 0;
}