#pragma once
#include "main.h"
#include "Player.h"
#include "Viewer.h"
#include "Board.h"
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_LBUTTON) & 0x01) ? 1:0)

class GameManager
{
public:
	Player* player[2];
	int current_player; // 0 white, 1 black
	Board board;
	Viewer viewer;
	string winner;
	clock_t timer;

	GameManager()
	{
		this->current_player = 0;
		player[0] = new HumanPlayer('W');
		player[1] = new HumanPlayer('B');
		this->winner = "";
	}

	void changePlayer()
	{
		if (current_player == 0)
			current_player = 1;
		else
			current_player = 0;
	}

	bool isMoveVaild(Position pos, vector <Position> list)
	{
		for (int i = 0; i < list.size(); i++)
			if (list[i].x == pos.x && list[i].y == pos.y)
				return true;
		//cout << "���ʤ��X�k�I" << endl;
		return false;
	}

	bool reachEnd(Position pos, int player)
	{
		if (player == 0 && pos.y == 0) // ��
			return true;
		else if (player == 1 && pos.y == 7)
			return true;
		return false;
	}

	bool checkPlayer(Position pos, int player)
	{
		if (player == 0 && board.board[pos.y][pos.x].player == 'W') // ��
			return true;
		else if (player == 1 && board.board[pos.y][pos.x].player == 'B')
			return true;

		//cout << "�ӴѤl���O�A��" << endl;
		return false;
	}

	bool isPosValid(int x, int y)
	{
		if (x >= 0 && x <= 7 && y >= 0 && y <= 7)
			return true;
		return false;
	}

	bool isInList(Position pos, vector<Position>list)
	{
		for (int i = 0; i < list.size(); i++)
			if (pos.x == list[i].x && pos.y == list[i].y)
				return true;
		return false;
	}

	bool isCheckMate()
	{
		if (!board.isBlackKingDanger() && !board.isWhiteKingDanger())
			return false;

		char player = (current_player == 0 ? 'W' : 'B'); // �ݯण���
		bool result = true;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (board.board[i][j].player == player)
				{
					Position pos;
					pos.x = j, pos.y = i;
					vector<Position> list = board.getValidPos(pos); // �Ѥl�i�����|
					// �C�@�B���չL
					for (int k = 0; k < list.size(); k++)
					{
						if (current_player == testMove(pos, list[k]))
							result = result && true;
						else
							result = result && false;
					}
				}
			}
		}
		if (result)
			winner = (player == 'W' ? "Black" : "White");

		return result;
	}

	bool isNothingCanMove()
	{
		if (board.isBlackKingDanger() || board.isWhiteKingDanger())
			return false;
		
		char player = (current_player == 0 ? 'W' : 'B'); // �ݯण���
		bool result = true;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (board.board[i][j].player == player)
				{
					Position pos;
					pos.x = j, pos.y = i;
					vector<Position> list = board.getValidPos(pos); // �Ѥl�i�����|
					// �C�@�B���չL
					for (int k = 0; k < list.size(); k++)
					{
						if (current_player == testMove(pos, list[k]))
							result = result && true;
						else
							result = result && false;
					}
				}
			}
		}
		if (result)
			winner = "Draw";
		
		return result;
	}

	bool clickMyChess(Position pos)
	{
		if (!isPosValid(pos.x, pos.y))
			return false;

		if (current_player == 0)
		{
			if (board.board[pos.y][pos.x].player == 'W')
				return true;
		}
		else
		{
			if (board.board[pos.y][pos.x].player == 'B')
				return true;
		}
		return false;
	}

	void specialPawn(Position pos1, Position pos2)
	{
		if (current_player == 0) // White
		{
			if (pos1.y == 6 && pos2.y == 4 && board.board[pos2.y][pos2.x].type == "Pawn")
			{
				// ���k
				for (int i = -1; i < 2; i += 2)
				{
					if (isPosValid(pos2.x + i, pos2.y))
					{
						if (board.board[pos2.y][pos2.x + i].type == "Pawn" && board.board[pos2.y][pos2.x + i].player == 'B')
							board.board[pos2.y][pos2.x + i].specialPawn = -i;
					}
				}
			}
		}
		else if (current_player == 1)
		{
			if (pos1.y == 1 && pos2.y == 3 && board.board[pos2.y][pos2.x].type == "Pawn")
			{
				// ���k
				for (int i = -1; i < 2; i += 2)
				{
					if (isPosValid(pos2.x + i, pos2.y))
					{
						if (board.board[pos2.y][pos2.x + i].type == "Pawn" && board.board[pos2.y][pos2.x + i].player == 'W')
							board.board[pos2.y][pos2.x + i].specialPawn = -i;
					}
				}
			}
		}
	}

	/*void printDisplayBoard(vector<Position> list)
	{
		char displayBoard[8][8];
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				displayBoard[i][j] = ' ';

		for (int i = 0; i < list.size(); i++)
			displayBoard[list[i].y][list[i].x] = 'X';

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (displayBoard[i][j] == ' ')
					displayBoard[i][j] = board.board[i][j].icon;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (board.board[i][j].player == 'B')
				{
					if (board.board[i][j].isDanger)
					{
						SetConsoleTextAttribute(hConsole, 12);
						cout << displayBoard[i][j];
					}
					else
					{
						SetConsoleTextAttribute(hConsole, 11);
						cout << displayBoard[i][j];
					}
				}
				else if (board.board[i][j].player == 'W')
				{
					if (board.board[i][j].isDanger)
					{
						SetConsoleTextAttribute(hConsole, 12);
						cout << displayBoard[i][j];
					}
					else
					{
						SetConsoleTextAttribute(hConsole, 7);
						cout << displayBoard[i][j];
					}
				}
				else
				{
					SetConsoleTextAttribute(hConsole, 14);
					cout << displayBoard[i][j];
				}
			}
			cout << endl;
		}
	}*/

	Position getMousePosition()
	{
		Position pos;
		while (1)
		{
			if (KEY_DOWN(MOUSE_EVENT))
			{
				POINT p;
				if (GetCursorPos(&p))
				{
					// Y �_�l�� 23 �@�� 113
					// X �_�l��  0 �@�� 77
					// cout << p.x << " " << p.y << endl;
					if (p.x >= 623 && p.x <= 699 && p.y >= 302 && p.y <= 414)
					{
						pos = { 10,0 };
						return pos;
					}
					pos.x = p.x / 77;
					pos.y = (p.y - 22) / 112;

					//cout << pos.x << " " << pos.y << endl;
					if (pos.x > 7 || pos.y > 7)
						continue;

					return pos;
				}
			}
		}
	}

	// �}�W�ߪO�l�ը��ݬݷ|���|�ɭP���Q�N�x
	// �^�ǭȬ��u�n�Q�N�x�N�^��player�N�X
	int testMove(Position fromPos, Position toPos)
	{
		Board newboard;

		newboard.board = board.board;
		newboard.blackKing = board.blackKing;
		newboard.whiteKing = board.whiteKing;

		player[current_player]->OnMove(newboard, fromPos, toPos);

		newboard.update();

		if (current_player == 0)
		{
			if (newboard.isWhiteKingDanger())
				return 0;
			if (newboard.isBlackKingDanger())
				return 1;
		}
		else
		{
			if (newboard.isBlackKingDanger())
				return 1;
			if (newboard.isWhiteKingDanger())
				return 0;
		}
	}

	void gameLoop()
	{
		while (1)
		{
			vector<Position> list;
			Position pos, pos2;
			pos2.x = 8, pos2.y = 8;

			if (isNothingCanMove())
			{
				viewer.printResult(board, winner);
				_getch();
				break;
			}
			if (isCheckMate())
			{
				viewer.printResult(board, winner);
				_getch();
				break;
			}

			while(1)
			{
				viewer.printBoard(board);
				viewer.printNowPlayer(current_player);

				if (clickMyChess(pos2))
					pos = pos2;
				else
					pos = getMousePosition();

				if (pos.x == 10 && pos.y == 0)// �뭰
				{
					winner = (current_player == 0 ? "Black" : "White");
					viewer.printResult(board, winner);
					_getch();
					break;
				}

				if (!checkPlayer(pos, current_player)) // �T�{�Ѥl�O���O�ۤv��
					continue;

				list = board.getValidPos(pos); // �i�����|
				if (list.empty())
				{
					// ����
					pos2.x = 8, pos2.y = 8;
					continue;
				}

				viewer.printDisplayBoard(board, list);
				viewer.printNowPlayer(current_player);

				pos2 = getMousePosition();

				if (clickMyChess(pos2)) // �I�ۤv���� = ���Ѥl��
					continue;

				while (!isMoveVaild(pos2, list))
				{
					// ���o�X�k��m
					pos2 = getMousePosition();
				}

				// ���]���ʫ�
				if (current_player == testMove(pos, pos2))
					continue;

				break;
			}
			
			if (winner != "") // �w�g��Ĺ�a
			{
				break;
			}
			
			player[current_player]->OnMove(board, pos, pos2);

			if (board.board[pos2.y][pos2.x].type == "Pawn") // �ʤɨƥ�
			{
				if (reachEnd(pos2, current_player))
				{
					Position pos;
					string str;
					viewer.printPromoteList((current_player == 0) ? 'W' : 'B');
					pos = getMousePosition();

					if (pos.x == 2 && pos.y == 3)
						str = "Rook";
					else if (pos.x == 3 && pos.y == 3)
						str = "Knight";
					else if (pos.x == 4 && pos.y == 3)
						str = "Bishop";
					else if (pos.x == 5 && pos.y == 3)
						str = "Queen";

					player[current_player]->OnPromote(board, pos2, str);
				}
			}

			board.update(); // �@�ǭ��m
			specialPawn(pos, pos2); // �L���L�ƥ�
			changePlayer();
		}
	}

	int menu()
	{
		char selection;
		viewer.printMenu();
		cin >> selection;

		if (selection == '1')
			return 1;
		else if(selection == '0')
		{
			system("cls");
			gameLoop();
		}
	}
};