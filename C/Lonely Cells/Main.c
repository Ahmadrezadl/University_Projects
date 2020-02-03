#include "Functions.h"

typedef struct cell{
	char name[20];
	int x;
	int y;
	bool moved;
	char team;
	int energy;
	struct cell * next;
}cell;

typedef struct cells{
	struct cell * head;
}Cells;

typedef struct block{
	int type;
	int energy;
	int x;
	int y;
}block; 

bool mp;
int X = 0;
int Y = 0;
cell * choosedCell = NULL;
Cells cells;

enum direction{north=1,south,northeast,northwest,southeast,southwest};
enum blocks{ Energy=1,Mitosis,Forbidden,Normal};

void printMap();
void printData();
void drawCell();
void init_map();
bool checkEmpty();
void clearToolTip(int n);
void printDirections();
void checkTurn(char team);
void printMoves(int n);
void placeCell(cell * newCell);
cell* addSplitedCell(int n,block map[n][n],char team,int x,int y);
cell * addCell(int n,block map[n][n],char team);
int menu();
void removeCell();
int main(){
	system("COLOR F0");
	srand(time(NULL));
	while(1)
	{
		mp = false; // Multiplayer hast ya na?
		cells.head = NULL;
		system("cls");
		bool load = false;
		int m = menu();
		if(m == 0 || m== 1 || m == 2)
		{
			if(m == 0)
			load = true;
			if(m == 2)
			mp = true;
			system("cls");
			char fileAdress[300];
			int n;
			enterMap:
			if(!load)
			{

				printf("Enter map adress: ");
				scanf("%s" , fileAdress);
				
			}
			else
			{
				strcpy(fileAdress,"map.sav");
			}
			FILE * f = fopen(fileAdress,"rb");
			if(f == NULL)
			{
				
				
				if(load)
				{
					printf("Error ! You haven's any save'\n");
					load = false;
					goto enterMap;
				}
				else printf("Error ! Cannot load map.\n");
				goto enterMap;
			}
			fread(&n, sizeof(unsigned int), 1, f);
			block map[n][n];
			int em = 0; //chanta khalie too map
			if(load)
			{
				for(int i = 0;i < n;i++)
				{
					for(int j = 0;j < n;j++)
					{
						fread(&map[i][j], sizeof(block), 1, f);
					}
				}
				fclose(f);
				printMap(n,map);
				FILE * f2 = fopen("cells.sav" , "rb");
				int tempCells;
				fread(&tempCells, sizeof(int), 1, f2);
				while(tempCells--)
				{
					cell * newCell = (cell*)malloc(sizeof(cell));
					fread(newCell, sizeof(cell), 1, f2);
					if(newCell->team == 'Y')
						mp=true;
					placeCell(newCell);
				}
				
				
			}
			else
			{
				for(int i = 0;i < n;i++)
				{
					for(int j = 0;j < n;j++)
					{
						char type;
						fread(&type, sizeof(unsigned char), 1, f);
						if(type != '3')em++;
						map[i][j].type = type - '0';
						map[i][j].energy = 100;
						map[i][j].x = i;
						map[i][j].y = j;
					}
				}
				system("cls");
				int c;
				printf("Enter number of cells: ");
				scanf("%d" , &c);
				while(c > em || (mp && em*2<c))
				{
					printf("Enter a lower Number: ");
					scanf("%d" , &c);
				}
				system("cls");
				printMap(n,map);
				for(int i = 0;i < c;i++)
				{
					cell * newC = addCell(n,map,'X');
					newC->moved = false;
					if(mp){
						newC = addCell(n,map,'Y');
						newC->moved = true;
					}
				}
			}
			fclose(f);

			X = 0;
			Y = 0;
			gotoxy(X,Y);
			//Game starts-------------------------------------------------------------------------
			bool endgame = false;
			while(1)
			{
				if(endgame == true)break;
				printData(X,Y,n,map);
				gotoxy(X,Y);
				bool endGame = false;
				switch(getKey())
				{
					case down:
						if(Y== n-1)break;
						Y += 1;
						printData(X,Y,n,map);
						gotoxy(X,Y);
						break;
					case up:
						if(Y ==0)break;
						Y -= 1;
						printData(X,Y,n,map);
						gotoxy(X,Y);
						break;
					case left:
						if(X == 0)break;
						X -= 1;
						printData(X,Y,n,map);
						gotoxy(X,Y);
						break;
					case right: 
						if(X == n-1)break;
						X += 1;
						printData(X,Y,n,map);
						gotoxy(X,Y);
						break;
					case esc:
						endGame = true;
						int count[2];
						count[0] = 0;
						count[1] = 0;
						cell * current = cells.head;
						clearToolTip(n);
						while(current != NULL)
						{
							if(current->team == 'X')
							count[0] += current->energy;
							else
							count[1] += current->energy;
							current = current->next;
						}
						if(mp)
						{
							gotoinfo(n,2);
							printf("Player 1 score: %d" , count[0]);
							gotoinfo(n,3);
							printf("Player 2 score: %d" , count[1]);
						}
						else
						{
							gotoinfo(n,2);
							printf("Your score: %d" , count[0]);
						}
						endgame = true;
						sleep(5);
						break;
					case enter:
						if (choosedCell == NULL || choosedCell->moved)break;
						printMoves(n);
						bool available[n][n];
						char lastTeam = choosedCell->team;
						switch(getKey())
						{
							case 1:
								printDirections(n);
								int changeX = 0;
								int changeY = 0;
								while(changeX == 0 && changeY == 0)
								{
									switch(getKey())
									{
										case north:
											changeY = -1;
											break;
										case south:
											changeY = 1;
											break;
										case northeast:
											if(choosedCell->x%2 == 0) changeY = -1;
											changeX = 1;
											break;
										case northwest:
											if(choosedCell->x%2 == 0) changeY = -1;
											changeX = -1;
											break;
										case southeast:
											if(choosedCell->x%2 == 1) changeY = 1;
											changeX = 1;
											break;
										case southwest:
											if(choosedCell->x%2 == 1) changeY = 1;
											changeX = -1;
											break;
										default:
											changeX = 100;
									}
									if(changeX == 100)
									{
										gotoxy(X,Y);
										break;
									}
									if(checkEmpty(choosedCell->x+changeX,choosedCell->y+changeY,n,map))
									{
										gotoxy(choosedCell->x,choosedCell->y);
										printf(" ");
										choosedCell->x+= changeX;
										choosedCell->y+= changeY;
										choosedCell->moved = true;
										drawCell(*choosedCell);
										gotoxy(X,Y);
										break;
									}
									else
									{
										changeX = 0;
										changeY = 0;
										gotoinfo(n,7);
										red
										printf("Wrong move!");
										reset
										gotoxy(X,Y);
									}
								}
								break;
							case 2:
								if(map[X][n-Y-1].type != Mitosis || choosedCell->energy < 80) break;
								int cx = 0;
								int cy = 0;
								int empty = 0;
								
								for(int i =0;i<n;i++)for(int j = 0;j< n;j++) available[i][j] = false;

								cy = -1; if(checkEmpty(X+cx,Y+cy,n,map)){
									empty++;
									available[X+cx][Y+cy] =true;
								}
								
								cy = 1; if(checkEmpty(X+cx,Y+cy,n,map)){
									empty++;
									available[X+cx][Y+cy] =true;
								}
								
								if(choosedCell->x%2 == 0) cy = -1; else cy = 0; cx = 1; if(checkEmpty(X+cx,Y+cy,n,map)){
									empty++;
									available[X+cx][Y+cy] =true;
								}
								
								if(choosedCell->x%2 == 0) cy = -1; else cy = 0; cx = -1; if(checkEmpty(X+cx,Y+cy,n,map)){
									empty++;
									available[X+cx][Y+cy] =true;
								}
								
								
								if(choosedCell->x%2 == 1) cy = 1; else cy = 0;cx = 1; if(checkEmpty(X+cx,Y+cy,n,map)){
									empty++;
									available[X+cx][Y+cy] =true;
								}
								
								if(choosedCell->x%2 == 1) cy = 1; else cy = 0;cx = -1; if(checkEmpty(X+cx,Y+cy,n,map)){
									empty++;
									available[X+cx][Y+cy] =true;
								}
								
								if(empty < 2)break;
								gotoxy(X,Y);
								printf(" \b");
								int spawned = 0;
								choosedCell->moved = true;
								while(spawned != 2)
								{
									int r = rand()%empty;
									bool cr = false;
									spawned++;
									for(int k = 0;k< n;k++)
									{
										for(int j = 0;j < n;j++)
										{
											if(available[k][j])
											{
												if(r > 0)
												{
													r--;
													continue;
												}
												available[k][j] = false;
												addSplitedCell(n,map,choosedCell->team,k,j);
												empty--;
												cr = true;
												break;
											}
										}
										if(cr)
										{
											cr = false;
											break;
										}
									}
									
								}
								removeCell(choosedCell);
								break;
							case 3:
								if(map[X][n-Y-1].type != Energy || map[X][n-Y-1].energy == 0 || choosedCell->energy >= 100){
									break;
								}
								map[X][Y].energy-=15;
								choosedCell->energy+=15;
								choosedCell->moved = true;
								if(map[X][Y].energy < 0)
								{
									choosedCell->energy += map[X][Y].energy;
									map[X][Y].energy = 0;
								}
								if(choosedCell->energy > 100)
								{
									map[X][Y].energy += choosedCell->energy - 100;
									choosedCell->energy = 100;
								}
								break;
							case 4:;
								FILE*  mFile = fopen("map.sav","wb");
								FILE*  cFile = fopen("cells.sav","wb");
								fwrite(&n,sizeof(int),1,mFile);
								for(int i = 0;i < n;i++)
								for(int j = 0;j < n;j++)
								{
									fwrite(&map[i][j],sizeof(block),1,mFile);
								}
								cell * current = cells.head;
								int numOfCells = 0;
								while(current != NULL)
								{
									numOfCells++;
									current = current->next;
								}
								fwrite(&numOfCells,sizeof(int),1,cFile);
								current = cells.head;
								while(current != NULL)
								{
									fwrite(current,sizeof(cell),1,cFile);
									current = current->next;
								}
								fclose(mFile);
								fclose(cFile);
						}
//						if(!mp)
//						choosedCell->moved = false;
						checkTurn(lastTeam);
						printData(X,Y,n,cells,map);
						clearToolTip(n);
				}
			}
			
			
		}
		if(m == 3)
			{
				system("cls");
				printf("Welcome to map editor!\nEnter n: ");
				int N;
				scanf("%d" , &N);
				unsigned char h[N][N];
				for(int i = 0 ; i < N; i++)
				{
					for(int j = 0; j< N;j++)
					{
						printf("Enter (%d.%d) block type:" , j,N-i-1);
						scanAgain:
						scanf("%c" , &h[i][j]);
						if(h[i][j] - '0' > 4 || h[i][j] - '0' < 1)
						goto scanAgain;
					}
				}
				printf("Enter file name: ");
				char filename[100];
				fflush(stdin);
				scanf("%s" , filename);
				FILE * fp = fopen(filename,"wb");
				fwrite(&N , sizeof(int) , 1,fp);
				for(int i = 0 ; i < N; i++)
				{
					for(int j = 0; j< N;j++)
					{
						fwrite(&h[i][j],sizeof(unsigned char),1,fp);
					}
				}
				fclose(fp);
			}
		
	}
}

void printData(int x, int y, int n,block map[n][n])
{
	clearToolTip(n);
	gotoinfo(n,-3);
	printf("%s" , map[x][m(y)].type==Energy?"Energy":map[x][m(y)].type==Mitosis?"Mitosis": map[x][m(y)].type==Forbidden?"Forbidden ":"Normal ");
	if(map[x][m(y)].type == Energy)
	printf("%d/100  ",map[X][Y].energy);
	gotoinfo(n,-2);
	printf("__________________________");
	gotoinfo(n,-1);
	cell * current = cells.head;
	while(current!=NULL)
	{
		if(current->x == x && current->y == y )
		{
			printf("%s / Energy: %d / Position: (%d,%d)  " , current->name , current->energy , current->x , current->y);
			choosedCell = current;
			gotoinfo(n,0);
			if(choosedCell->moved)
			{
				red
				printf("This cell is moved");
				reset
			}
			else
			{	
				green
				printf("Press Enter...");
				reset
			}
			break;
		}
		if(current->next == NULL)
		{
			printf("                                                           ");
			choosedCell = NULL;
			break;
		}
		else
		current = current->next;
	}
}

bool checkEmpty(int x, int y,int n,block map[n][n])
{
	if(x == -1 || y == -1 || x == n || y == n) return false;
	if(map[x][n-y-1].type == Forbidden)return false;
	cell * current = cells.head;
	while(current != NULL)
	{
		if(current->x == x && current->y == y)
		return false;
		current = current->next;
	}
	return true;
}

void printLog(char* s,int n)
{
	gotoinfo(n,9);
	printf("%s" , s);
}

void printMoves(int n)
{
	gotoinfo(n,1);
	printf("1) Move         ");
	gotoinfo(n,2);
	printf("2) Split a cell ");
	gotoinfo(n,3);
	printf("3) Boost energy ");
	gotoinfo(n,4);
	printf("4) Save");
	gotoxy(X,Y);
}

void printDirections(int n)
{
	gotoinfo(n,1);
	printf("1) North        ");
	gotoinfo(n,2);
	printf("2) South        ");
	gotoinfo(n,3);
	printf("3) NorthEast    ");
	gotoinfo(n,4);
	printf("4) NorthWest    ");
	gotoinfo(n,5);
	printf("5) SouthEast    ");
	gotoinfo(n,6);
	printf("6) SouthWest    ");
	gotoxy(X,Y);
	
}

void removeCell(cell * x)
{
	cell * current = cells.head;
	if(x == current)
	{
		cells.head = x->next;
		free(x);
		
	}
	else
	{
		while(current->next != x && current != NULL)
		{
			current = current->next;
		}
		cell * willRemove = current->next;
		current->next = current->next->next;
		free(willRemove);
	}
}

void clearToolTip(int n)
{
	for(int i = -3;i<9;i++)
	{
		gotoinfo(n,i);
		printf("                  ");
	}
	
}



void drawCell(cell* x)
{
	gotoxy(x->x,x->y);
	printf("%c\b",x->team);
}

void checkTurn(char team)
{
	cell * current = cells.head;
	while(current!=NULL)
	{
		if(current->moved == false)
		{
			return;
		}
		current = current->next;
	}
	
	current = cells.head;
	while(current!=NULL)
	{
		if(current->moved && current->team != team)
		{
			current->moved = false;
		}
		else if(!mp)
		{
			current->moved = false;
		}
		current = current->next;
	}
}

cell* addCell(int n,block map[n][n],char team){
		cell * newCell = (cell*) malloc(sizeof(cell));
		int x = rand()%n;
		int y = rand()%n;
		while(!checkEmpty(x,y,n,map))
		{
			x = rand()%n;
			y = rand()%n;
		}
		newCell->energy = 40;
		newCell->x = x;
		newCell->y = y;
		newCell->next = NULL;
		newCell->team = team;
		newCell->moved = false;
		strcpy(newCell->name,randstring());
		if(cells.head == NULL)
		cells.head = newCell;
		else
		{
			cell * currentCell = cells.head;
			while(currentCell->next != NULL)
			currentCell = currentCell->next;
			currentCell->next = newCell;
		}
		drawCell(newCell);
		return newCell;
}

void placeCell(cell* newCell)
{
	newCell->next = NULL;
	if(cells.head == NULL)
		cells.head = newCell;
	else
	{
		cell * currentCell = cells.head;
		while(currentCell->next != NULL)
		currentCell = currentCell->next;
		currentCell->next = newCell;
	}
	drawCell(newCell);
}

cell* addSplitedCell(int n,block map[n][n],char team,int x,int y){
		cell * newCell = (cell*) malloc(sizeof(cell));
		newCell->energy = 40;
		newCell->x = x;
		newCell->y = y;
		newCell->next = NULL;
		newCell->team = team;
		newCell->moved = false;
		strcpy(newCell->name,randstring());
		if(cells.head == NULL)
		cells.head = newCell;
		else
		{
			cell * currentCell = cells.head;
			while(currentCell->next != NULL)
			currentCell = currentCell->next;
			currentCell->next = newCell;
		}
		drawCell(newCell);
		return newCell;
}

void gt(int eex, int eey){
    COORD coord;
    coord.X = eex;
    coord.Y = eey;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


void printMap(int n,block map[n][n]){	
	for(int i = 0;i < n;i++)
		{
			for(int j = 0 ; j < n;j++)
			{
				int x = j;
				int y = i;
				if(x%2 == 0)
				{
					x*=8;
					y*=6;
					gt(x,y);
					gt(x,y);
					printf("________");
					gt(x,y+1);
					if(map[j][n-i-1].type == Energy)
					printf("Energy");
					else if(map[j][n-i-1].type == Mitosis)
					printf("Mitosis");
					else if(map[j][n-i-1].type == Forbidden)
					printf("Forbiddn");
					else
					printf("Normal");
					gt(x,y+6);
					printf("________");
					
				}
				else
				{
					x*=8;
					y*=6;
					y += 4;
					gt(x,y);
					printf("________");
					gt(x,y+1);
					if(map[j][n-i-1].type == Energy)
					printf("Energy  ");
					else if(map[j][n-i-1].type == Mitosis)
					printf("Mitosis ");
					else if(map[j][n-i-1].type == Forbidden)
					printf("Forbidden ");
					else
					printf("Normal  ");
					gt(x,y+6);
					printf("________");
				}
			}
		}
		
		gotoinfo(n,10);
		printf("NW    N    NE");
		gotoinfo(n,11);
		printf("            ");
		gotoinfo(n,12);
		printf("W     *     E");
		gotoinfo(n,13);
		printf("            ");
		gotoinfo(n,14);
		printf("SW    S    SE");
		
}

int menu(){
	int choose = 0;
	while(1)
	{
		system("@cls||clear");
		printf("Menu: \n");
		printf("_________________________\n");
		printf("| 1%c Load                |\n" , choose == 0?'>':' ');
		printf("|________________________|\n");
		printf("| 2%c New single player   |\n" , choose == 1?'>':' ');
		printf("|________________________|\n");
		printf("| 3%c New Multiplayer     |\n" , choose == 2?'>':' ');
		printf("|________________________|\n");
		printf("| 4%c Map Editor          |\n" , choose == 3?'>':' ');
		printf("|________________________|\n");
		printf("| 5%c Exit                |\n" , choose == 4?'>':' ');
		printf("|________________________|\n");
		while(1)
		{
			int ch = getKey();
			if(ch == up && choose != 0)
			{
				choose--;
				break;
			}
			if(ch == down && choose != 4)
			{
				choose++;
				break;
			}
			if(ch == enter)
			{
				return choose;
			}
			if(ch == esc)return quit;
			if(ch < 6) return ch-1;
			
		}
	}
}
