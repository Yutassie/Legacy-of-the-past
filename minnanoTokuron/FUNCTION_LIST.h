#include "GLOBAL_VARIABLE.h"

void file_input(void)
{
	int input;

	fp1 = fopen(fname1, "r");
	if (fp1 == NULL){
		printf("%s入力ファイルが開けません\n", fname1);
		exit(-1);
	}

	fscanf(fp1, "%d %d %d %d %d", &width, &height,
		&times,
		&selCost, &swapCost);

	//初期化
	for (j = 0; j<16; j++){
		for (i = 0; i<16; i++){
			tile[i][j].x = -1;
			tile[i][j].y = -1;
			tile[i][j].status = -1;
		}
	}

	//代入
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			fscanf(fp1, "%d", &input);
			tile[j][i].x = input / 10;
			tile[j][i].y = input % 10;
			tile[i][j].status = -1;
		}
	}
	fclose(fp1);
}

void position_output(int x, int y)
{
	fp2 = fopen(fname2, "a");
	if (fp2 == NULL){
		printf("%s出力ファイルが開けません\n", fname2);
		exit(-1);
	}

	fprintf(fp2, "%x%x\n", x, y);//選択画像位置

	fclose(fp2);

	sel++;
}

void swap_output(void)
{
	fp2 = fopen(fname2, "a");
	if (fp2 == NULL){
		printf("%s出力ファイルが開けません\n", fname2);
		exit(-1);
	}

	fprintf(fp2, "%d\n%s\n", swap, sousa);//交換回数

	fclose(fp2);
	line_clear();
}

void line_clear(void)
{
	for (i = 0; i<swap; i++){
		sousa[i] = NULL;
	}
	swap = 0;
}

int select(int x, int y){//場所（ｘ，ｙ）のタイルをつかむ失敗したら-1を返す。
	if (x>0xF && y>0xF){
		return -1;
	}
	selected.x = x;
	selected.y = y;
	selected.status = 0;

	position_output(x, y);//場所を出力

	return 0;
}

int Im_select(int x, int y){//場所（ｘ，ｙ）のタイルをつかむ失敗したら-1を返す。
	if (x>0xF && y>0xF){
		return -1;
	}
	Im_selected.x = x;
	Im_selected.y = y;
	Im_selected.status = 0;

	return 0;
}

void set_masu(int Qx, int Qy){
	int i, j;
	int ran_x, ran_y;
	struct masu temp[16][16];

	srand((unsigned)time(NULL));

	for (j = 0; j<16; j++){
		for (i = 0; i<16; i++){
			tile[i][j].x = -1;
			tile[i][j].y = -1;
			tile[i][j].status = -1;
		}
	}

	for (j = 0; j<Qy; j++){
		for (i = 0; i<Qx; i++){
			tile[i][j].x = i;
			tile[i][j].y = j;
			tile[i][j].status = -1;

		}
	}

	for (j = 0; j<Qy; j++){
		for (i = 0; i<Qx; i++){
			ran_x = rand() % Qx;
			ran_y = rand() % Qy;
			temp[i][j].x = tile[ran_x][ran_y].x;
			tile[ran_x][ran_y].x = tile[i][j].x;
			tile[i][j].x = temp[i][j].x;

			temp[i][j].y = tile[ran_x][ran_y].y;
			tile[ran_x][ran_y].y = tile[i][j].y;
			tile[i][j].y = temp[i][j].y;
		}
	}
}

struct masu search_tile(int x, int y){
	int i, j;
	struct masu null = { -1, -1, -1 };

	for (j = 0; j<16; j++){
		for (i = 0; i<16; i++){
			if (tile[i][j].x == x){
				if (tile[i][j].y == y){
					null.x = i;
					null.y = j;
					return null;
				}
			}
		}
	}

	return null;
}

struct masu Im_search_tile(int x, int y){
	int i, j;
	struct masu null = { -1, -1, -1 };

	for (j = 0; j<16; j++){
		for (i = 0; i<16; i++){
			if (Im_tile[i][j].x == x){
				if (Im_tile[i][j].y == y){
					null.x = i;
					null.y = j;
					return null;
				}
			}
		}
	}

	return null;
}

int move_R(int selected_x, int selected_y){
	struct masu temp;

	if (selected_x + 1 == width)
		return -1;
	if (tile[selected_x + 1][selected_y].status == 1)
		return -1;

	if (selected_x + 1 == moved_tile.x && selected_y == moved_tile.y){
		moved_tile.x = moved_tile.x - 1;
	}

	temp = tile[selected_x + 1][selected_y];
	//printf("%d %d\n", searched_tile.x, searched_tile.y);
	tile[selected_x + 1][selected_y] = tile[selected_x][selected_y];
	tile[selected_x][selected_y] = temp;

	selected.x = selected.x + 1;

	sousa[swap++] = 'R';

	return 0;
}

int move_L(int selected_x, int selected_y){
	struct masu temp;

	if (selected_x == 0)
		return -1;
	if (tile[selected_x - 1][selected_y].status == 1)
		return -1;

	if (selected_x - 1 == moved_tile.x && selected_y == moved_tile.y){
		moved_tile.x = moved_tile.x + 1;
	}

	temp = tile[selected_x - 1][selected_y];
	tile[selected_x - 1][selected_y] = tile[selected_x][selected_y];
	tile[selected_x][selected_y] = temp;

	selected.x = selected.x - 1;

	sousa[swap++] = 'L';

	return 0;
}

int move_U(int selected_x, int selected_y){
	struct masu temp;

	if (selected_y == 0)
		return -1;
	if (tile[selected_x][selected_y - 1].status == 1)
		return -1;

	if (selected_y - 1 == moved_tile.y && selected_x == moved_tile.x){
		moved_tile.y = moved_tile.y + 1;
	}

	temp = tile[selected_x][selected_y - 1];
	tile[selected_x][selected_y - 1] = tile[selected_x][selected_y];
	tile[selected_x][selected_y] = temp;

	selected.y = selected.y - 1;

	sousa[swap++] = 'U';

	return 0;
}

int move_D(int selected_x, int selected_y){
	struct masu temp;

	if (selected_y + 1 == height)
		return -1;
	if (selected_y + 1 == moved_tile.y && selected_x == moved_tile.x){
		moved_tile.y = moved_tile.y - 1;

	}

	temp = tile[selected_x][selected_y + 1];
	tile[selected_x][selected_y + 1] = tile[selected_x][selected_y];
	tile[selected_x][selected_y] = temp;

	selected.y = selected.y + 1;

	sousa[swap++] = 'D';

	return 0;
}

int Im_move_R(int selected_x, int selected_y){
	struct masu temp;

	if (selected_x + 1 == width)
		return -1;
	if (Im_tile[selected_x + 1][selected_y].status == 1)
		return -1;

	if (selected_x + 1 == Im_moved_tile.x && selected_y == Im_moved_tile.y){
		Im_moved_tile.x = Im_moved_tile.x - 1;
	}

	temp = Im_tile[selected_x + 1][selected_y];
	//printf("%d %d\n", searched_tile.x, searched_tile.y);
	Im_tile[selected_x + 1][selected_y] = Im_tile[selected_x][selected_y];
	Im_tile[selected_x][selected_y] = temp;

	Im_selected.x = Im_selected.x + 1;

	return 0;
}

int Im_move_L(int selected_x, int selected_y){
	struct masu temp;

	if (selected_x == 0)
		return -1;
	if (Im_tile[selected_x - 1][selected_y].status == 1)
		return -1;

	if (selected_x - 1 == Im_moved_tile.x && selected_y == Im_moved_tile.y){
		Im_moved_tile.x = Im_moved_tile.x + 1;
	}

	temp = Im_tile[selected_x - 1][selected_y];
	Im_tile[selected_x - 1][selected_y] = Im_tile[selected_x][selected_y];
	Im_tile[selected_x][selected_y] = temp;

	Im_selected.x = Im_selected.x - 1;

	return 0;
}

int Im_move_U(int selected_x, int selected_y){
	struct masu temp;

	if (selected_y == 0)
		return -1;
	if (Im_tile[selected_x][selected_y - 1].status == 1)
		return -1;

	if (selected_y - 1 == Im_moved_tile.y && selected_x == Im_moved_tile.x){
		Im_moved_tile.y = Im_moved_tile.y + 1;
	}

	temp = Im_tile[selected_x][selected_y - 1];
	Im_tile[selected_x][selected_y - 1] = Im_tile[selected_x][selected_y];
	Im_tile[selected_x][selected_y] = temp;

	Im_selected.y = Im_selected.y - 1;

	return 0;
}

int Im_move_D(int selected_x, int selected_y){
	struct masu temp;

	if (selected_y + 1 == height)
		return -1;
	if (selected_y + 1 == Im_moved_tile.y && selected_x == Im_moved_tile.x){
		Im_moved_tile.y = Im_moved_tile.y - 1;

	}

	temp = Im_tile[selected_x][selected_y + 1];
	Im_tile[selected_x][selected_y + 1] = Im_tile[selected_x][selected_y];
	Im_tile[selected_x][selected_y] = temp;

	Im_selected.y = Im_selected.y + 1;

	return 0;
}

int tile_move(int goal_x, int goal_y){
	int doubleinflag = 0;
	
	//動かしたいタイル
	moved_tile = search_tile(goal_x, goal_y);

	//printf("選択：%d%d ", selected.x, selected.y);
	//printf("対象：%d%d\n", moved_tile.x, moved_tile.y);

	///////////////////////////////////////////////////////
	//各行最後の2個は一気に入れるため別処理
	if (goal_x == width - 1 && goal_y != height - 3){
		goal_y = goal_y + 1;
		doubleinflag = 2;
	}
	else if (goal_x == width - 2 && goal_y != height - 3){
		goal_x = goal_x + 1;
		doubleinflag = 1;
	}
	if (doubleinflag != 0){
		//揃えられないパターンを検出し別のパターンで揃える
		if (moved_tile.x + 1 == goal_x && moved_tile.y == goal_y - 1){
			while (selected.x != goal_x - 1){
				if (selected.x < goal_x - 1)
					move_R(selected.x, selected.y);
				else
					move_L(selected.x, selected.y);
			}
			while (selected.y != goal_y)
				move_U(selected.x, selected.y);

			move_U(selected.x, selected.y);
			move_R(selected.x, selected.y);
			move_D(selected.x, selected.y);
			move_D(selected.x, selected.y);
			move_L(selected.x, selected.y);
			move_U(selected.x, selected.y);
			move_R(selected.x, selected.y);
			move_U(selected.x, selected.y);
			move_L(selected.x, selected.y);
			move_D(selected.x, selected.y);
			move_D(selected.x, selected.y);
			move_R(selected.x, selected.y);
			move_U(selected.x, selected.y);
			move_L(selected.x, selected.y);
			move_U(selected.x, selected.y);
			move_R(selected.x, selected.y);
			move_D(selected.x, selected.y);

			tile[goal_x - 1][goal_y - 1].status = 1;
			tile[goal_x][goal_y - 1].status = 1;

			return 1;
		}


		//すでにそろっているときは処理を飛ばす
		if (moved_tile.x != goal_x || moved_tile.y != goal_y){
			if (selected.x == moved_tile.x){
				if (selected.x == width - 1){
					move_L(selected.x, selected.y);
					moveLflag = 1;
				}
				else
					move_R(selected.x, selected.y);
			}

			if (selected.y < moved_tile.y){
				while (selected.y < moved_tile.y){
					move_D(selected.x, selected.y);
				}

			}
			else if (selected.y > moved_tile.y){
				while (selected.y != moved_tile.y){
					if (tile[selected.x][selected.y - 1].status == 1){
						move_R(selected.x, selected.y);
					}
					else{
						if (selected.x == moved_tile.x)
							move_R(selected.x, selected.y);
						move_U(selected.x, selected.y);
					}
				}
			}
			else;

			if (moveLflag == 1){
				move_R(selected.x, selected.y);
				moveLflag = 0;
			}
			//printf("同行：%d%d ", selected.x, selected.y);
			//printf("対象：%d%d \n", moved_tile.x, moved_tile.y);
			//動かしたいタイルの右に
			if (selected.x < moved_tile.x){
				while (selected.x < moved_tile.x){
					if (tile[selected.x + 1][selected.y].status == 1){
						move_D(selected.x, selected.y);
						moveDflag = 1;
					}
					move_R(selected.x, selected.y);
					if (moveDflag == 1 && tile[selected.x][selected.y - 1].status != 1){
						move_U(selected.x, selected.y);
						moveDflag = 0;
					}
				}
			}
			else if (selected.x > moved_tile.x){
				while (selected.x > moved_tile.x + 1){
					if (tile[selected.x - 1][selected.y].status == 1){
						move_D(selected.x, selected.y);
						moveDflag = 1;
					}
					move_L(selected.x, selected.y);
					if (moveDflag == 1 && tile[selected.x][selected.y - 1].status != 1){
						move_U(selected.x, selected.y);
						moveDflag = 0;
					}
				}
			}
			//同じ行に行けなかったときに実行
			if (selected.y > moved_tile.y){
				move_L(selected.x, selected.y);
				move_U(selected.x, selected.y);
				move_R(selected.x, selected.y);
			}
			//
			//printf("右列：%d%d ", selected.x, selected.y);
			//printf("対象：%d%d \n", moved_tile.x, moved_tile.y);

			//行または列が合うまで斜め移動をする。
			if (goal_y != height - 2){
				if (moved_tile.x >= goal_x){
					while (moved_tile.x != goal_x && moved_tile.y != goal_y){
						move_U(selected.x, selected.y);
						move_L(selected.x, selected.y);
						move_D(selected.x, selected.y);
						if (moved_tile.y == goal_y){
							//行がそろった時点で列がそろっていたら
							if (moved_tile.x == goal_x)
								breakflag = 1;
							else{
								move_L(selected.x, selected.y);
								move_U(selected.x, selected.y);
								move_R(selected.x, selected.y);
							}
							breakflag = 1;
						}
						if (breakflag == 1){
							breakflag = 0;
							break;
						}
						move_L(selected.x, selected.y);
						move_U(selected.x, selected.y);
						move_R(selected.x, selected.y);
						if (moved_tile.x == goal_x){
							move_U(selected.x, selected.y);
							move_L(selected.x, selected.y);
							move_D(selected.x, selected.y);
							breakflag = 1;
						}
						if (breakflag == 1){
							breakflag = 0;
							break;
						}
					}
				}
				if (moved_tile.x < goal_x){
					//最後から二番目の行を揃えるとき、こうなっていたら揃えられないので
					if (goal_y == height - 2){
						//後から選択するために保存
						next_select.x = goal_x;
						next_select.y = goal_y;
						return 0;
					}

					move_L(selected.x, selected.y);
					if (moved_tile.x == goal_x){
						//すでにそろってたら何もしない
						if (moved_tile.y == goal_y){
							breakflag = 1;
						}
						if (breakflag != 1){
							move_U(selected.x, selected.y);
							move_R(selected.x, selected.y);
							move_D(selected.x, selected.y);
						}
						if (breakflag == 1)
						{
							breakflag = 0;
						}
					}
					while (moved_tile.x != goal_x && moved_tile.y != goal_y){
						//if→上に移動できない場合
						if (tile[selected.x][selected.y - 1].status == 1 || selected.y == 0){
							breakflag = 1;
						}
						else{
							move_U(selected.x, selected.y);
							move_R(selected.x, selected.y);
							move_D(selected.x, selected.y);
						}
						if (breakflag == 1){
							breakflag = 0;
							break;
						}

						move_R(selected.x, selected.y);
						move_U(selected.x, selected.y);
						move_L(selected.x, selected.y);
						if (moved_tile.x == goal_x){
							move_U(selected.x, selected.y);
							move_R(selected.x, selected.y);
							move_D(selected.x, selected.y);
							breakflag = 1;
						}
						if (breakflag == 1){
							breakflag = 0;
							break;
						}
					}
				}
				//printf("斜め：%d%d ", selected.x, selected.y);
				//printf("対象：%d%d \n", moved_tile.x, moved_tile.y);
			}

			//動かしたいタイルが一番0下の段にあるとき１段上に動かしたいタイルをうごかす。
			if (moved_tile.y + 1 == height){
				move_U(selected.x, selected.y);
				move_L(selected.x, selected.y);
				move_D(selected.x, selected.y);
				move_R(selected.x, selected.y);
				move_U(selected.x, selected.y);
			}
			//動かしたいタイルを目標地点の列に合わせる。
			if (moved_tile.x < goal_x){
				//最後から二番目の行を揃えるとき、こうなっていたら揃えられないので
				if (goal_y == height - 2){
					//後から選択するために保存
					next_select.x = goal_x;
					next_select.y = goal_y;
					return 0;
				}

				if (selected.x == moved_tile.x + 1)
					move_L(selected.x, selected.y);
				while (moved_tile.x != goal_x){
					move_D(selected.x, selected.y);
					move_R(selected.x, selected.y);
					move_R(selected.x, selected.y);
					move_U(selected.x, selected.y);
					move_L(selected.x, selected.y);
				}
				if (tile[selected.x][selected.y - 1].status == 1 || selected.y == 0){
					move_D(selected.x, selected.y);
					move_R(selected.x, selected.y);
				}
				else{
					move_U(selected.x, selected.y);
					move_R(selected.x, selected.y);
					move_D(selected.x, selected.y);
				}
			}
			//printf("合列：%d%d 　対象物：%d%d\n", selected.x, selected.y, moved_tile.x, moved_tile.y);

			//動かしたいタイルを目標地点の行に合わせる。
			while (moved_tile.y != goal_y){
				move_L(selected.x, selected.y);
				move_U(selected.x, selected.y);
				move_U(selected.x, selected.y);
				move_R(selected.x, selected.y);
				move_D(selected.x, selected.y);
			}
			//printf("合行：%d%d \n", selected.x, selected.y);
		}

		if (doubleinflag == 2){
			if (tile[goal_x - 1][goal_y - 1].x == goal_x - 1 && tile[goal_x - 1][goal_y - 1].y == goal_y - 1){
				if (tile[goal_x][goal_y - 1].x == goal_x && tile[goal_x][goal_y - 1].y == goal_y - 1){
					tile[goal_x - 1][goal_y - 1].status = 1;
					tile[goal_x][goal_y - 1].status = 1;
				}
			}
			else if(selected.x == moved_tile.x && selected.y == moved_tile.y + 1){
				move_L(selected.x, selected.y);
				move_U(selected.x, selected.y);
				move_U(selected.x, selected.y);
				move_R(selected.x, selected.y);
				move_D(selected.x, selected.y);
				tile[goal_x - 1][goal_y - 1].status = 1;
				tile[goal_x][goal_y - 1].status = 1;
			}
			else
			{
				while (selected.x != moved_tile.x - 1){
					if (selected.x < moved_tile.x - 1)
						move_R(selected.x, selected.y);
					else
						move_L(selected.x, selected.y);
				}
				while (selected.y != moved_tile.y - 1){
					move_U(selected.x, selected.y);
				}
				move_R(selected.x, selected.y);
				move_D(selected.x, selected.y);
				tile[goal_x - 1][goal_y - 1].status = 1;
				tile[goal_x][goal_y - 1].status = 1;
			}

			return 0;
		}


		return 0;
	}
	//////////////////////////////////////////////////////////////////

	//すでにそろっているとき
	if (moved_tile.x == goal_x && moved_tile.y == goal_y){
		tile[goal_x][goal_y].status = 1;
		return 0;
	}

	//揃えられないパターンを検出
	if (goal_y == height - 2){
		if (tile[moved_tile.x - 1][moved_tile.y].status == 1){
			return 0;
		}
	}
	//最後の列に選択していたタイルをおく
	if (goal_x == width - 1){
		while (goal_x != selected.x){
			if (tile[selected.x + 1][selected.y].status == 1)
				move_D(selected.x, selected.y);
			move_R(selected.x, selected.y);
		}
		while (goal_y != selected.y)
			move_U(selected.x, selected.y);

		tile[goal_x][goal_y].status = 1;
		return 0;
	}

	//選択したタイルのy座標の位置が揃えたい行にあるとき下に移動。
	if (goal_y == selected.y){
		move_D(selected.x, selected.y);
	}
	//動かしたいタイルと同じ行に移動
	if (selected.x == moved_tile.x){
		if (selected.x == width - 1){
			move_L(selected.x, selected.y);
			moveLflag = 1;
		}
		else
			move_R(selected.x, selected.y);
	}

	if (selected.y < moved_tile.y){
		while (selected.y < moved_tile.y){
			move_D(selected.x, selected.y);
		}

	}
	else if (selected.y > moved_tile.y){
		while (selected.y != moved_tile.y){
			if (tile[selected.x][selected.y - 1].status == 1){
				move_R(selected.x, selected.y);
			}
			else{
				if (selected.x == moved_tile.x)
					move_R(selected.x, selected.y);
				move_U(selected.x, selected.y);
			}
		}
	}
	else;

	if (moveLflag == 1){
		move_R(selected.x, selected.y);
		moveLflag = 0;
	}
	//printf("同行：%d%d ", selected.x, selected.y);
	//printf("対象：%d%d \n", moved_tile.x, moved_tile.y);
	//動かしたいタイルの右に
	if (selected.x < moved_tile.x){
		while (selected.x < moved_tile.x){
			if (tile[selected.x + 1][selected.y].status == 1){
				move_D(selected.x, selected.y);
				moveDflag = 1;
			}
			move_R(selected.x, selected.y);
			if (moveDflag == 1 && tile[selected.x][selected.y - 1].status != 1){
				move_U(selected.x, selected.y);
				moveDflag = 0;
			}
		}
	}
	else if (selected.x > moved_tile.x){
		while (selected.x > moved_tile.x + 1){
			if (tile[selected.x - 1][selected.y].status == 1){
				move_D(selected.x, selected.y);
				moveDflag = 1;
			}
			move_L(selected.x, selected.y);
			if (moveDflag == 1 && tile[selected.x][selected.y - 1].status != 1){
				move_U(selected.x, selected.y);
				moveDflag = 0;
			}
		}
	}
	//同じ行に行けなかったときに実行
	if (selected.y > moved_tile.y){
		move_L(selected.x, selected.y);
		move_U(selected.x, selected.y);
		move_R(selected.x, selected.y);
	}
	//
	//printf("右列：%d%d ", selected.x, selected.y);
	//printf("対象：%d%d \n", moved_tile.x, moved_tile.y);

	//行または列が合うまで斜め移動をする。
	if (goal_y != height - 2){
		if (moved_tile.x >= goal_x){
			while (moved_tile.x != goal_x && moved_tile.y != goal_y){
				move_U(selected.x, selected.y);
				move_L(selected.x, selected.y);
				move_D(selected.x, selected.y);
				if (moved_tile.y == goal_y){
					//行がそろった時点で列がそろっていたら
					if (moved_tile.x == goal_x)
						breakflag = 1;
					else{
						move_L(selected.x, selected.y);
						move_U(selected.x, selected.y);
						move_R(selected.x, selected.y);
					}
					breakflag = 1;
				}
				if (breakflag == 1){
					breakflag = 0;
					break;
				}
				move_L(selected.x, selected.y);
				move_U(selected.x, selected.y);
				move_R(selected.x, selected.y);
				if (moved_tile.x == goal_x){
					move_U(selected.x, selected.y);
					move_L(selected.x, selected.y);
					move_D(selected.x, selected.y);
					breakflag = 1;
				}
				if (breakflag == 1){
					breakflag = 0;
					break;
				}
			}
		}
		if (moved_tile.x < goal_x){
			//最後から二番目の行を揃えるとき、こうなっていたら揃えられないので
			if (goal_y == height - 2){
				//後から選択するために保存
				next_select.x = goal_x;
				next_select.y = goal_y;
				return 0;
			}

			move_L(selected.x, selected.y);
			if (moved_tile.x == goal_x){
				//if→上に移動できない場合
				if (tile[selected.x][selected.y - 1].status == 1 || selected.y == 0){
					move_D(selected.x, selected.y);
					move_R(selected.x, selected.y);
					move_R(selected.x, selected.y);
					move_U(selected.x, selected.y);
					move_U(selected.x, selected.y);
					move_L(selected.x, selected.y);
					move_D(selected.x, selected.y);
				}
				else{
					move_U(selected.x, selected.y);
					move_R(selected.x, selected.y);
					move_D(selected.x, selected.y);
				}

			}
			while (moved_tile.x != goal_x && moved_tile.y != goal_y){
				//if→上に移動できない場合
				if (tile[selected.x][selected.y - 1].status == 1 || selected.y == 0){
					breakflag = 1;
				}
				else{
					move_U(selected.x, selected.y);
					move_R(selected.x, selected.y);
					move_D(selected.x, selected.y);
				}
				if (breakflag == 1){
					breakflag = 0;
					break;
				}

				move_R(selected.x, selected.y);
				move_U(selected.x, selected.y);
				move_L(selected.x, selected.y);
				if (moved_tile.x == goal_x){
					//if→上に移動できない場合
					if (tile[selected.x][selected.y - 1].status == 1 || selected.y == 0){
						move_D(selected.x, selected.y);
						move_R(selected.x, selected.y);
						move_R(selected.x, selected.y);
						move_U(selected.x, selected.y);
						move_U(selected.x, selected.y);
						move_L(selected.x, selected.y);
						move_D(selected.x, selected.y);
						breakflag = 1;
					}
					else{
						move_U(selected.x, selected.y);
						move_R(selected.x, selected.y);
						move_D(selected.x, selected.y);
						breakflag = 1;
					}
				}
				if (breakflag == 1){
					breakflag = 0;
					break;
				}
			}
		}
		//printf("斜め：%d%d ", selected.x, selected.y);
		//printf("対象：%d%d \n", moved_tile.x, moved_tile.y);
	}
	//動かしたいタイルが一番0下の段にあるとき１段上に動かしたいタイルをうごかす。
	if (moved_tile.y + 1 == height){
		move_U(selected.x, selected.y);
		move_L(selected.x, selected.y);
		move_D(selected.x, selected.y);
		move_R(selected.x, selected.y);
		move_U(selected.x, selected.y);
	}
	//動かしたいタイルを目標地点の列に合わせる。
	if (moved_tile.x >= goal_x){
		while (moved_tile.x != goal_x){
			move_D(selected.x, selected.y);
			move_L(selected.x, selected.y);
			move_L(selected.x, selected.y);
			move_U(selected.x, selected.y);
			move_R(selected.x, selected.y);
		}
		//if→上に移動できない場合
		if (moved_tile.x == goal_x && moved_tile.y == goal_y){
			tile[goal_x][goal_y].status = 1;
			return 0;
		}
		if (tile[selected.x][selected.y - 1].status == 1 || selected.y == 0){
			move_D(selected.x, selected.y);
			move_L(selected.x, selected.y);
		}
		else{
			if (selected.x - 1 == moved_tile.x){
				move_U(selected.x, selected.y);
				move_L(selected.x, selected.y);
				move_D(selected.x, selected.y);
			}
		}
	}
	if (moved_tile.x < goal_x){
		//最後から二番目の行を揃えるとき、こうなっていたら揃えられないので
		if (goal_y == height - 2){
			//後から選択するために保存
			next_select.x = goal_x;
			next_select.y = goal_y;
			return 0;
		}

		if (selected.x == moved_tile.x + 1)
			move_L(selected.x, selected.y);
		while (moved_tile.x != goal_x){
			move_D(selected.x, selected.y);
			move_R(selected.x, selected.y);
			move_R(selected.x, selected.y);
			move_U(selected.x, selected.y);
			move_L(selected.x, selected.y);
		}
		if (tile[selected.x][selected.y - 1].status == 1 || selected.y == 0){
			move_D(selected.x, selected.y);
			move_R(selected.x, selected.y);
		}
		else{
			move_U(selected.x, selected.y);
			move_R(selected.x, selected.y);
			move_D(selected.x, selected.y);
		}
	}
	//printf("合列：%d%d 　対象物：%d%d\n", selected.x, selected.y, moved_tile.x, moved_tile.y);

	//動かしたいタイルを目標地点の行に合わせる。
	while (moved_tile.y != goal_y){
		move_R(selected.x, selected.y);
		move_U(selected.x, selected.y);
		move_U(selected.x, selected.y);
		move_L(selected.x, selected.y);
		move_D(selected.x, selected.y);
	}
	//printf("合行：%d%d \n", selected.x, selected.y);

	tile[goal_x][goal_y].status = 1;

	return 0;
}

int tile_shita2gyo_sort(int goal_x, int goal_y){//下の行2行をソート
	//動かしたいタイル
	moved_tile = search_tile(goal_x, goal_y);

	//printf("選択：%d%d ", selected.x, selected.y);
	//printf("対象：%d%d\n", moved_tile.x, moved_tile.y);

	//本来入れたい場所の下に置いておき最後に二つを一気に揃える
	if (goal_y == height - 2){
		goal_y = goal_y + 1;

		//すぐ終わりのパターン
		if (selected.x == moved_tile.x && moved_tile.y + 1 == goal_y){
			move_U(selected.x, selected.y);
			move_R(selected.x, selected.y);
			move_D(selected.x, selected.y);
		}
		//動かしたいタイルと同じ行に移動
		if (selected.x == moved_tile.x){
			if (selected.x == width - 1){
				move_L(selected.x, selected.y);
				moveLflag = 1;
			}
			else
				move_R(selected.x, selected.y);
		}

		if (selected.y < moved_tile.y){
			while (selected.y < moved_tile.y){
				move_D(selected.x, selected.y);
			}

		}
		else if (selected.y > moved_tile.y){
			while (selected.y != moved_tile.y){
				if (tile[selected.x][selected.y - 1].status == 1){
					move_R(selected.x, selected.y);
				}
				else{
					if (selected.x == moved_tile.x)
						move_R(selected.x, selected.y);
					move_U(selected.x, selected.y);
				}
			}
		}
		else;

		if (moveLflag == 1){
			move_R(selected.x, selected.y);
			moveLflag = 0;
		}
		//printf("同行：%d%d ", selected.x, selected.y);
		//printf("対象：%d%d \n", moved_tile.x, moved_tile.y);
		//動かしたいタイルの右に
		if (selected.x < moved_tile.x){
			while (selected.x < moved_tile.x){
				if (tile[selected.x + 1][selected.y].status == 1){
					move_D(selected.x, selected.y);
					moveDflag = 1;
				}
				move_R(selected.x, selected.y);
				if (moveDflag == 1 && tile[selected.x][selected.y - 1].status != 1){
					move_U(selected.x, selected.y);
					moveDflag = 0;
				}
			}
		}
		else if (selected.x > moved_tile.x){
			while (selected.x > moved_tile.x + 1){
				if (tile[selected.x - 1][selected.y].status == 1){
					move_D(selected.x, selected.y);
					moveDflag = 1;
				}
				move_L(selected.x, selected.y);
				if (moveDflag == 1 && tile[selected.x][selected.y - 1].status != 1){
					move_U(selected.x, selected.y);
					moveDflag = 0;
				}
			}
		}
		//同じ行に行けなかったときに実行
		if (selected.y > moved_tile.y){
			move_L(selected.x, selected.y);
			move_U(selected.x, selected.y);
			move_R(selected.x, selected.y);
		}
		//
		//printf("右列：%d%d ", selected.x, selected.y);
		//printf("対象：%d%d \n", moved_tile.x, moved_tile.y);

		//行・列合わせ
		if (moved_tile.y == height - 2){
			move_D(selected.x, selected.y);
			move_L(selected.x, selected.y);
			move_U(selected.x, selected.y);
			if (moved_tile.x != goal_x){
				move_L(selected.x, selected.y);
				move_D(selected.x, selected.y);
				move_R(selected.x, selected.y);
			}
			else
				move_R(selected.x, selected.y);
		}
		//printf("合行：%d%d ", selected.x, selected.y);
		//printf("対象：%d%d \n", moved_tile.x, moved_tile.y);
		while (moved_tile.x != goal_x){
			move_U(selected.x, selected.y);
			move_L(selected.x, selected.y);
			move_L(selected.x, selected.y);
			move_D(selected.x, selected.y);
			move_R(selected.x, selected.y);
		}
		//printf("合列：%d%d ", selected.x, selected.y);
		//printf("対象：%d%d \n", moved_tile.x, moved_tile.y);
	}
	else{
		goal_x = goal_x + 1;
		//すぐ終わるパターン
		if (selected.x == moved_tile.x && moved_tile.y + 1 == goal_y){
			move_U(selected.x, selected.y);
			move_L(selected.x, selected.y);
			move_D(selected.x, selected.y);
			move_R(selected.x, selected.y);

			tile[goal_x - 1][goal_y].status = 1;
			tile[goal_x - 1][goal_y - 1].status = 1;

			return 1;
		}
		//動かしたいタイルと同じ行に移動
		if (selected.x == moved_tile.x){
			if (selected.x == width - 1){
				move_L(selected.x, selected.y);
				moveLflag = 1;
			}
			else
				move_R(selected.x, selected.y);
		}

		if (selected.y < moved_tile.y){
			while (selected.y < moved_tile.y){
				move_D(selected.x, selected.y);
			}

		}
		else if (selected.y > moved_tile.y){
			while (selected.y != moved_tile.y){
				if (tile[selected.x][selected.y - 1].status == 1){
					move_R(selected.x, selected.y);
				}
				else{
					if (selected.x == moved_tile.x)
						move_R(selected.x, selected.y);
					move_U(selected.x, selected.y);
				}
			}
		}
		else;

		if (moveLflag == 1){
			move_R(selected.x, selected.y);
			moveLflag = 0;
		}
		//printf("同行：%d%d ", selected.x, selected.y);
		//printf("対象：%d%d \n", moved_tile.x, moved_tile.y);
		//動かしたいタイルの右に
		if (selected.x < moved_tile.x){
			while (selected.x < moved_tile.x){
				if (tile[selected.x + 1][selected.y].status == 1){
					move_D(selected.x, selected.y);
					moveDflag = 1;
				}
				move_R(selected.x, selected.y);
				if (moveDflag == 1 && tile[selected.x][selected.y - 1].status != 1){
					move_U(selected.x, selected.y);
					moveDflag = 0;
				}
			}
		}
		else if (selected.x > moved_tile.x){
			while (selected.x > moved_tile.x + 1){
				if (tile[selected.x - 1][selected.y].status == 1){
					move_D(selected.x, selected.y);
					moveDflag = 1;
				}
				move_L(selected.x, selected.y);
				if (moveDflag == 1 && tile[selected.x][selected.y - 1].status != 1){
					move_U(selected.x, selected.y);
					moveDflag = 0;
				}
			}
		}
		//同じ行に行けなかったときに実行
		if (selected.y > moved_tile.y){
			move_L(selected.x, selected.y);
			move_U(selected.x, selected.y);
			move_R(selected.x, selected.y);
		}
		//
		//printf("右列：%d%d ", selected.x, selected.y);
		//printf("対象：%d%d \n", moved_tile.x, moved_tile.y);

		//揃えられないパターンを検出し別のパターンで揃える
		if (moved_tile.y + 1 == goal_y && moved_tile.x == goal_x - 1){
			if (selected.y != goal_y)
				move_D(selected.x, selected.y);

			move_L(selected.x, selected.y);
			move_U(selected.x, selected.y);
			move_R(selected.x, selected.y);
			move_D(selected.x, selected.y);
			move_R(selected.x, selected.y);
			move_U(selected.x, selected.y);
			move_L(selected.x, selected.y);
			move_L(selected.x, selected.y);
			move_D(selected.x, selected.y);
			move_R(selected.x, selected.y);
			move_U(selected.x, selected.y);
			move_R(selected.x, selected.y);
			move_D(selected.x, selected.y);
			move_L(selected.x, selected.y);
			move_L(selected.x, selected.y);
			move_U(selected.x, selected.y);
			move_R(selected.x, selected.y);

			tile[goal_x - 1][goal_y].status = 1;
			tile[goal_x - 1][goal_y - 1].status = 1;

			return 1;
		}

		//行・列合わせ
		if (moved_tile.y == height - 2){
			move_D(selected.x, selected.y);
			move_L(selected.x, selected.y);
			move_U(selected.x, selected.y);
			if (moved_tile.x != goal_x){
				move_L(selected.x, selected.y);
				move_D(selected.x, selected.y);
				move_R(selected.x, selected.y);
			}
			//ここで揃えられる
			else{
				move_L(selected.x, selected.y);
				move_D(selected.x, selected.y);
				move_R(selected.x, selected.y);
				tile[goal_x - 1][goal_y].status = 1;
				tile[goal_x - 1][goal_y - 1].status = 1;

				return 1;
			}
		}
		//printf("合行：%d%d ", selected.x, selected.y);
		//printf("対象：%d%d \n", moved_tile.x, moved_tile.y);
		while (moved_tile.x != goal_x){
			move_U(selected.x, selected.y);
			move_L(selected.x, selected.y);
			move_L(selected.x, selected.y);
			move_D(selected.x, selected.y);
			move_R(selected.x, selected.y);
		}
		//printf("合列：%d%d ", selected.x, selected.y);
		//printf("対象：%d%d \n", moved_tile.x, moved_tile.y);
		//二個同時に入れる。
		move_U(selected.x, selected.y);
		move_L(selected.x, selected.y);
		move_L(selected.x, selected.y);
		move_D(selected.x, selected.y);
		move_R(selected.x, selected.y);
		tile[goal_x - 1][goal_y].status = 1;
		tile[goal_x - 1][goal_y - 1].status = 1;
	}
	return 1;
}

int Im_tile_shita2gyo_sort(int goal_x, int goal_y){//下の行2行をソート
	//動かしたいタイル
	Im_moved_tile = Im_search_tile(goal_x, goal_y);

	//printf("選択：%d%d ", Im_selected.x, Im_selected.y);
	//printf("対象：%d%d\n", Im_moved_tile.x, Im_moved_tile.y);

	//本来入れたい場所の下に置いておき最後に二つを一気に揃える
	if (goal_y == height - 2){
		goal_y = goal_y + 1;
		//動かしたいタイルと同じ行に移動
		///////////
		if (Im_selected.x == Im_moved_tile.x && Im_moved_tile.y + 1 == goal_y){
			Im_move_U(Im_selected.x, Im_selected.y);
			Im_move_R(Im_selected.x, Im_selected.y);
			Im_move_D(Im_selected.x, Im_selected.y);
		}
		///////////
		if (Im_selected.x == Im_moved_tile.x){
			if (Im_selected.x == width - 1){
				Im_move_L(Im_selected.x, Im_selected.y);
				moveLflag = 1;
			}
			else
				Im_move_R(Im_selected.x, Im_selected.y);
		}

		if (Im_selected.y < Im_moved_tile.y){
			while (Im_selected.y < Im_moved_tile.y){
				Im_move_D(Im_selected.x, Im_selected.y);
			}

		}
		else if (Im_selected.y > Im_moved_tile.y){
			while (Im_selected.y != Im_moved_tile.y){
				if (Im_tile[Im_selected.x][Im_selected.y - 1].status == 1){
					Im_move_R(Im_selected.x, Im_selected.y);
				}
				else{
					if (Im_selected.x == Im_moved_tile.x)
						Im_move_R(Im_selected.x, Im_selected.y);
					Im_move_U(Im_selected.x, Im_selected.y);
				}
			}
		}
		else;

		if (moveLflag == 1){
			Im_move_R(Im_selected.x, Im_selected.y);
			moveLflag = 0;
		}
		//printf("同行：%d%d ", Im_selected.x, Im_selected.y);
		//printf("対象：%d%d \n", Im_moved_tile.x, Im_moved_tile.y);
		//動かしたいタイルの右に
		if (Im_selected.x < Im_moved_tile.x){
			while (Im_selected.x < Im_moved_tile.x){
				if (Im_tile[Im_selected.x + 1][Im_selected.y].status == 1){
					Im_move_D(Im_selected.x, Im_selected.y);
					moveDflag = 1;
				}
				Im_move_R(Im_selected.x, Im_selected.y);
				if (moveDflag == 1 && Im_tile[Im_selected.x][Im_selected.y - 1].status != 1){
					Im_move_U(Im_selected.x, Im_selected.y);
					moveDflag = 0;
				}
			}
		}
		else if (Im_selected.x > Im_moved_tile.x){
			while (Im_selected.x > Im_moved_tile.x + 1){
				if (Im_tile[Im_selected.x - 1][Im_selected.y].status == 1){
					Im_move_D(Im_selected.x, Im_selected.y);
					moveDflag = 1;
				}
				Im_move_L(Im_selected.x, Im_selected.y);
				if (moveDflag == 1 && Im_tile[Im_selected.x][Im_selected.y - 1].status != 1){
					Im_move_U(Im_selected.x, Im_selected.y);
					moveDflag = 0;
				}
			}
		}
		//同じ行に行けなかったときに実行
		if (Im_selected.y > Im_moved_tile.y){
			Im_move_L(Im_selected.x, Im_selected.y);
			Im_move_U(Im_selected.x, Im_selected.y);
			Im_move_R(Im_selected.x, Im_selected.y);
		}
		//
		//printf("右列：%d%d ", Im_selected.x, Im_selected.y);
		//printf("対象：%d%d \n", Im_moved_tile.x, Im_moved_tile.y);

		//行・列合わせ
		if (Im_moved_tile.y == height - 2){
			Im_move_D(Im_selected.x, Im_selected.y);
			Im_move_L(Im_selected.x, Im_selected.y);
			Im_move_U(Im_selected.x, Im_selected.y);
			if (Im_moved_tile.x != goal_x){
				Im_move_L(Im_selected.x, Im_selected.y);
				Im_move_D(Im_selected.x, Im_selected.y);
				Im_move_R(Im_selected.x, Im_selected.y);
			}
			else
				Im_move_R(Im_selected.x, Im_selected.y);
		}
		//printf("合行：%d%d ", Im_selected.x, Im_selected.y);
		//printf("対象：%d%d \n", Im_moved_tile.x, Im_moved_tile.y);
		while (Im_moved_tile.x != goal_x){
			Im_move_U(Im_selected.x, Im_selected.y);
			Im_move_L(Im_selected.x, Im_selected.y);
			Im_move_L(Im_selected.x, Im_selected.y);
			Im_move_D(Im_selected.x, Im_selected.y);
			Im_move_R(Im_selected.x, Im_selected.y);
		}
		//printf("合列：%d%d ", Im_selected.x, Im_selected.y);
		//printf("対象：%d%d \n", Im_moved_tile.x, Im_moved_tile.y);
	}
	else{
		goal_x = goal_x + 1;
		//動かしたいタイルと同じ行に移動
		///////////あとでIｍじゃない方に足す
		if (Im_selected.x == Im_moved_tile.x && Im_moved_tile.y + 1 == goal_y){
			Im_move_U(Im_selected.x, Im_selected.y);
			Im_move_L(Im_selected.x, Im_selected.y);
			Im_move_D(Im_selected.x, Im_selected.y);
			Im_move_R(Im_selected.x, Im_selected.y);

			Im_tile[goal_x - 1][goal_y].status = 1;
			Im_tile[goal_x - 1][goal_y - 1].status = 1;

			return 1;
		}
		///////////
		if (Im_selected.x == Im_moved_tile.x){
			if (Im_selected.x == width - 1){
				Im_move_L(Im_selected.x, Im_selected.y);
				moveLflag = 1;
			}
			else
				Im_move_R(Im_selected.x, Im_selected.y);
		}

		if (Im_selected.y < Im_moved_tile.y){
			while (Im_selected.y < Im_moved_tile.y){
				Im_move_D(Im_selected.x, Im_selected.y);
			}

		}
		else if (Im_selected.y > Im_moved_tile.y){
			while (Im_selected.y != Im_moved_tile.y){
				if (Im_tile[Im_selected.x][Im_selected.y - 1].status == 1){
					Im_move_R(Im_selected.x, Im_selected.y);
				}
				else{
					if (Im_selected.x == Im_moved_tile.x)
						Im_move_R(Im_selected.x, Im_selected.y);
					Im_move_U(Im_selected.x, Im_selected.y);
				}
			}
		}
		else;

		if (moveLflag == 1){
			Im_move_R(Im_selected.x, Im_selected.y);
			moveLflag = 0;
		}
		//printf("同行：%d%d ", Im_selected.x, Im_selected.y);
		//printf("対象：%d%d \n", Im_moved_tile.x, Im_moved_tile.y);
		//動かしたいタイルの右に
		if (Im_selected.x < Im_moved_tile.x){
			while (Im_selected.x < Im_moved_tile.x){
				if (Im_tile[Im_selected.x + 1][Im_selected.y].status == 1){
					Im_move_D(Im_selected.x, Im_selected.y);
					moveDflag = 1;
				}
				Im_move_R(Im_selected.x, Im_selected.y);
				if (moveDflag == 1 && Im_tile[Im_selected.x][Im_selected.y - 1].status != 1){
					Im_move_U(Im_selected.x, Im_selected.y);
					moveDflag = 0;
				}
			}
		}
		else if (Im_selected.x > Im_moved_tile.x){
			while (Im_selected.x > Im_moved_tile.x + 1){
				if (Im_tile[Im_selected.x - 1][Im_selected.y].status == 1){
					Im_move_D(Im_selected.x, Im_selected.y);
					moveDflag = 1;
				}
				Im_move_L(Im_selected.x, Im_selected.y);
				if (moveDflag == 1 && Im_tile[Im_selected.x][Im_selected.y - 1].status != 1){
					Im_move_U(Im_selected.x, Im_selected.y);
					moveDflag = 0;
				}
			}
		}
		//同じ行に行けなかったときに実行
		if (Im_selected.y > Im_moved_tile.y){
			Im_move_L(Im_selected.x, Im_selected.y);
			Im_move_U(Im_selected.x, Im_selected.y);
			Im_move_R(Im_selected.x, Im_selected.y);
		}
		//
		//printf("右列：%d%d ", Im_selected.x, Im_selected.y);
		//printf("対象：%d%d \n", Im_moved_tile.x, Im_moved_tile.y);

		//揃えられないパターンを検出し別のパターンで揃える
		if (Im_moved_tile.y + 1 == goal_y && Im_moved_tile.x == goal_x - 1){
			if (Im_selected.y != goal_y)
				Im_move_D(Im_selected.x, Im_selected.y);

			Im_move_L(Im_selected.x, Im_selected.y);
			Im_move_U(Im_selected.x, Im_selected.y);
			Im_move_R(Im_selected.x, Im_selected.y);
			Im_move_D(Im_selected.x, Im_selected.y);
			Im_move_R(Im_selected.x, Im_selected.y);
			Im_move_U(Im_selected.x, Im_selected.y);
			Im_move_L(Im_selected.x, Im_selected.y);
			Im_move_L(Im_selected.x, Im_selected.y);
			Im_move_D(Im_selected.x, Im_selected.y);
			Im_move_R(Im_selected.x, Im_selected.y);
			Im_move_U(Im_selected.x, Im_selected.y);
			Im_move_R(Im_selected.x, Im_selected.y);
			Im_move_D(Im_selected.x, Im_selected.y);
			Im_move_L(Im_selected.x, Im_selected.y);
			Im_move_L(Im_selected.x, Im_selected.y);
			Im_move_U(Im_selected.x, Im_selected.y);
			Im_move_R(Im_selected.x, Im_selected.y);

			Im_tile[goal_x - 1][goal_y].status = 1;
			Im_tile[goal_x - 1][goal_y - 1].status = 1;

			return 1;
		}

		//行・列合わせ
		if (Im_moved_tile.y == height - 2){
			Im_move_D(Im_selected.x, Im_selected.y);
			Im_move_L(Im_selected.x, Im_selected.y);
			Im_move_U(Im_selected.x, Im_selected.y);
			if (Im_moved_tile.x != goal_x){
				Im_move_L(Im_selected.x, Im_selected.y);
				Im_move_D(Im_selected.x, Im_selected.y);
				Im_move_R(Im_selected.x, Im_selected.y);
			}
			//ここで揃えられる
			else{
				Im_move_L(Im_selected.x, Im_selected.y);
				Im_move_D(Im_selected.x, Im_selected.y);
				Im_move_R(Im_selected.x, Im_selected.y);
				Im_tile[goal_x - 1][goal_y].status = 1;
				Im_tile[goal_x - 1][goal_y - 1].status = 1;

				return 1;
			}
		}
		//printf("合行：%d%d ", Im_selected.x, Im_selected.y);
		//printf("対象：%d%d \n", Im_moved_tile.x, Im_moved_tile.y);
		while (Im_moved_tile.x != goal_x){
			Im_move_U(Im_selected.x, Im_selected.y);
			Im_move_L(Im_selected.x, Im_selected.y);
			Im_move_L(Im_selected.x, Im_selected.y);
			Im_move_D(Im_selected.x, Im_selected.y);
			Im_move_R(Im_selected.x, Im_selected.y);
		}
		//printf("合列：%d%d ", Im_selected.x, Im_selected.y);
		//printf("対象：%d%d \n", Im_moved_tile.x, Im_moved_tile.y);
		//二個同時に入れる。
		Im_move_U(Im_selected.x, Im_selected.y);
		Im_move_L(Im_selected.x, Im_selected.y);
		Im_move_L(Im_selected.x, Im_selected.y);
		Im_move_D(Im_selected.x, Im_selected.y);
		Im_move_R(Im_selected.x, Im_selected.y);
		Im_tile[goal_x - 1][goal_y].status = 1;
		Im_tile[goal_x - 1][goal_y - 1].status = 1;
	}
	return 1;
}

int judge_crcr(void){
	for (int i = width - 2; i < width; i++){
		for (int j = height - 2; j < height; j++){
			if (tile[i][j].x != i || tile[i][j].y != j)
				return 0;
		}
	}
	return 1;
}

int Im_judge_crcr(void){
	for (int i = width - 2; i < width; i++){
		for (int j = height - 2; j < height; j++){
			if (Im_tile[i][j].x != i || Im_tile[i][j].y != j)
				return 0;
		}
	}
	return 1;
}

int tile_curucuru(){
	for (int i = 0; i < 3; i++){
		if (selected.x == width - 1){
			if (selected.y == height - 1){
				move_L(selected.x, selected.y);
				if (judge_crcr())
					return 1;
				move_U(selected.x, selected.y);
				if (judge_crcr())
					return 1;
			}
			else{
				move_U(selected.x, selected.y);
				if (judge_crcr())
					return 1;
			}
		}
		else{
			if (selected.y == height - 1){
				move_U(selected.x, selected.y);
				if (judge_crcr())
					return 1;
			}
		}
		move_R(selected.x, selected.y);
		if (judge_crcr())
			return 1;
		move_D(selected.x, selected.y);
		if (judge_crcr())
			return 1;
		move_L(selected.x, selected.y);
		if (judge_crcr())
			return 1;
		move_U(selected.x, selected.y);
		if (judge_crcr())
			return 1;
	}
	return 0;
}

int Im_tile_curucuru(){
	for (int i = 0; i < 3; i++){
		if (Im_selected.x == width - 1){
			if (Im_selected.y == height - 1){
				Im_move_L(Im_selected.x, Im_selected.y);
				if (Im_judge_crcr())
					return 1;
				Im_move_U(Im_selected.x, Im_selected.y);
				if (Im_judge_crcr())
					return 1;
			}
			else{
				Im_move_U(Im_selected.x, Im_selected.y);
				if (Im_judge_crcr())
					return 1;
			}
		}
		else{
			if (Im_selected.y == height - 1){
				Im_move_U(Im_selected.x, Im_selected.y);
				if (Im_judge_crcr())
					return 1;
			}
		}
		Im_move_R(Im_selected.x, Im_selected.y);
		if (Im_judge_crcr())
			return 1;
		Im_move_D(Im_selected.x, Im_selected.y);
		if (Im_judge_crcr())
			return 1;
		Im_move_L(Im_selected.x, Im_selected.y);
		if (Im_judge_crcr())
			return 1;
		Im_move_U(Im_selected.x, Im_selected.y);
		if (Im_judge_crcr())
			return 1;
	}
	return 0;
}