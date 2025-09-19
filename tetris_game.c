#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define COLUMNS 14
#define ROWS 22

const int LEFT_MARGIN = 2;
const int CELL_WIDTH = 2;

int SCORE = 0;
int LEVEL = 0;

int stageInfo[ROWS][COLUMNS] = {0};

char blockModel[14][4][4] =
    {
        {{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
        {{0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
        {{1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
        {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}}};

// ----------------- UTILITY FUNCTIONS -----------------

SHORT cellToConsoleX(int column)
{
    return (SHORT)(LEFT_MARGIN + column * CELL_WIDTH);
}

void copyBlock(char destination[4][4], const char source[4][4])
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            destination[row][col] = source[row][col];
        }
    }
}

// ----------------- DRAW FUNCTIONS -----------------

void drawBorder(HANDLE console)
{
    COORD position;
    SHORT leftX = (SHORT)(LEFT_MARGIN - 1);
    SHORT rightX = (SHORT)(LEFT_MARGIN + COLUMNS * CELL_WIDTH);

    for (int row = 0; row <= ROWS; row++)
    {
        position.X = leftX;
        position.Y = (SHORT)row;
        SetConsoleCursorPosition(console, position);
        printf("|");

        position.X = rightX;
        position.Y = (SHORT)row;
        SetConsoleCursorPosition(console, position);
        printf("|");
    }

    for (SHORT x = leftX; x <= rightX; x++)
    {
        position.X = x;
        position.Y = (SHORT)ROWS;
        SetConsoleCursorPosition(console, position);
        printf("-");
    }
}

void drawStage(HANDLE console)
{
    COORD position;

    for (int row = 0; row < ROWS; row++)
    {
        for (int column = 0; column < COLUMNS; column++)
        {
            position.X = cellToConsoleX(column);
            position.Y = (SHORT)row;
            SetConsoleCursorPosition(console, position);

            if (stageInfo[row][column] == 1)
            {
                printf("��");
            }
            else
            {
                printf("  ");
            }
        }
    }
}

void drawBlock(HANDLE console, int X, int Y, char block[4][4])
{
    COORD position;

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (block[row][col])
            {
                int stageX = X + col;
                int stageY = Y + row;

                if (stageY >= 0 && stageY < ROWS && stageX >= 0 && stageX < COLUMNS)
                {
                    position.X = cellToConsoleX(stageX);
                    position.Y = (SHORT)stageY;
                    SetConsoleCursorPosition(console, position);
                    printf("��");
                }
            }
        }
    }
}

void clearBlock(HANDLE console, int X, int Y, char block[4][4])
{
    COORD position;

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (block[row][col])
            {
                int stageX = X + col;
                int stageY = Y + row;

                if (stageY >= 0 && stageY < ROWS && stageX >= 0 && stageX < COLUMNS)
                {
                    position.X = cellToConsoleX(stageX);
                    position.Y = (SHORT)stageY;
                    SetConsoleCursorPosition(console, position);
                    printf("  ");
                }
            }
        }
    }
}

void drawDISPLAYBorder(HANDLE console)
{
    COORD position;
    SHORT leftX = 26;
    SHORT rightX = 52;

    for (int row = 0; row <= ROWS; row++)
    {
        position.X = rightX;
        position.Y = (SHORT)row;
        SetConsoleCursorPosition(console, position);
        printf("|");
    }

    for (SHORT x = leftX; x <= rightX; x++)
    {
        position.X = x;
        position.Y = (SHORT)ROWS;
        SetConsoleCursorPosition(console, position);
        printf("-");
    }

    position.X = 38;
    position.Y = 2;
    SetConsoleCursorPosition(console, position);
    printf("Level %d", LEVEL);

    position.X = 38;
    position.Y = 4;
    SetConsoleCursorPosition(console, position);
    printf("Score %d", SCORE);
}

void drawNextBlock(HANDLE console, char block[4][4])
{
    COORD position;
    SHORT startX = 38;
    SHORT startY = 7;

    // Clear previous next block
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            position.X = (SHORT)(startX + col * CELL_WIDTH);
            position.Y = (SHORT)(startY + row);
            SetConsoleCursorPosition(console, position);
            printf("  ");
        }
    }

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            position.X = (SHORT)(startX + col * CELL_WIDTH);
            position.Y = (SHORT)(startY + row);
            SetConsoleCursorPosition(console, position);

            if (block[row][col])
            {
                printf("��");
            }
        }
    }
}

// ----------------- BLOCK LOGIC -----------------

void rotateBlock(char block[4][4])
{
    char temp[4][4];

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            temp[col][3 - row] = block[row][col];
        }
    }

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            block[row][col] = temp[row][col];
        }
    }
}

int checkCollision(int X, int Y, char block[4][4])
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (block[row][col])
            {
                int stageX = X + col;
                int stageY = Y + row;

                if (stageX < 0 || stageX >= COLUMNS)
                {
                    return 1;
                }

                if (stageY >= ROWS)
                {
                    return 1;
                }

                if (stageY >= 0 && stageInfo[stageY][stageX] == 1)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

void mergeBlocks(int X, int Y, char block[4][4])
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (block[row][col])
            {
                int stageX = X + col;
                int stageY = Y + row;

                if (stageY >= 0 && stageY < ROWS && stageX >= 0 && stageX < COLUMNS)
                {
                    stageInfo[stageY][stageX] = 1;
                }
            }
        }
    }
}

int clearLines()
{
    int clearedLines = 0;

    for (int row = ROWS - 1; row >= 0; row--)
    {
        int fullLine = 1;

        for (int col = 0; col < COLUMNS; col++)
        {
            if (stageInfo[row][col] == 0)
            {
                fullLine = 0;
                break;
            }
        }

        if (fullLine)
        {
            clearedLines++;

            for (int moveRow = row; moveRow > 0; moveRow--)
            {
                for (int col = 0; col < COLUMNS; col++)
                {
                    stageInfo[moveRow][col] = stageInfo[moveRow - 1][col];
                }
            }

            for (int col = 0; col < COLUMNS; col++)
            {
                stageInfo[0][col] = 0;
            }

            row++;
        }
    }

    return clearedLines;
}

// ----------------- MAIN -----------------

int main(void)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = 0;
    SetConsoleCursorInfo(console, &cursorInfo);

    // Set console buffer size to prevent cursor overflow
    COORD bufferSize;
    bufferSize.X = COLUMNS * CELL_WIDTH + LEFT_MARGIN + 10;
    bufferSize.Y = ROWS + 5;
    SetConsoleScreenBufferSize(console, bufferSize);

    srand((unsigned int)time(NULL));

    system("cls");
    drawBorder(console);
    drawDISPLAYBorder(console);
    drawStage(console);

    int currentBlockIndex = rand() % 14;
    int nextBlockIndex = rand() % 14;

    char currentBlock[4][4];
    copyBlock(currentBlock, blockModel[currentBlockIndex]);
    drawNextBlock(console, blockModel[nextBlockIndex]);

    int blockPosX = COLUMNS / 2 - 2;
    int blockPosY = 0;
    drawBlock(console, blockPosX, blockPosY, currentBlock);

    DWORD lastTick = GetTickCount();
    const DWORD DROP_INTERVAL = 500;

    while (1)
    {
        if (_kbhit())
        {
            int key = _getch();

            if (key == 224)
            {
                int arrow = _getch();

                if (arrow == 75 && !checkCollision(blockPosX - 1, blockPosY, currentBlock))
                {
                    clearBlock(console, blockPosX, blockPosY, currentBlock);
                    blockPosX--;
                    drawBlock(console, blockPosX, blockPosY, currentBlock);
                }
                else if (arrow == 77 && !checkCollision(blockPosX + 1, blockPosY, currentBlock))
                {
                    clearBlock(console, blockPosX, blockPosY, currentBlock);
                    blockPosX++;
                    drawBlock(console, blockPosX, blockPosY, currentBlock);
                }
                else if (arrow == 80 && !checkCollision(blockPosX, blockPosY + 1, currentBlock))
                {
                    clearBlock(console, blockPosX, blockPosY, currentBlock);
                    blockPosY++;
                    drawBlock(console, blockPosX, blockPosY, currentBlock);
                }
                else if (arrow == 72)
                {
                    char tempBlock[4][4];
                    copyBlock(tempBlock, currentBlock);
                    rotateBlock(tempBlock);

                    if (!checkCollision(blockPosX, blockPosY, tempBlock))
                    {
                        clearBlock(console, blockPosX, blockPosY, currentBlock);
                        copyBlock(currentBlock, tempBlock);
                        drawBlock(console, blockPosX, blockPosY, currentBlock);
                    }
                }
            }
            else if (key == 32) // Space bar drop
            {
                while (!checkCollision(blockPosX, blockPosY + 1, currentBlock))
                {
                    clearBlock(console, blockPosX, blockPosY, currentBlock);
                    blockPosY++;
                    drawBlock(console, blockPosX, blockPosY, currentBlock);
                }
            }
            else if (key == 27) // ESC key
            {
                break;
            }
        }

        DWORD now = GetTickCount();
        if (now - lastTick >= DROP_INTERVAL)
        {
            if (!checkCollision(blockPosX, blockPosY + 1, currentBlock))
            {
                clearBlock(console, blockPosX, blockPosY, currentBlock);
                blockPosY++;
                drawBlock(console, blockPosX, blockPosY, currentBlock);
            }
            else
            {
                // Merge and clear lines
                mergeBlocks(blockPosX, blockPosY, currentBlock);
                drawStage(console);
                int lines = clearLines();
                if (lines > 0)
                {
                    // Update score based on number of lines cleared at once
                    if (lines == 1)
                    {
                        SCORE += 40;
                    }
                    else if (lines == 2)
                    {
                        SCORE += 100;
                    }
                    else if (lines == 3)
                    {
                        SCORE += 300;
                    }
                    else if (lines >= 4) // optional for 4-line cleanup (Tetris)
                    {
                        SCORE += 1200;
                    }

                    if (SCORE >= 300)
                    {
                        LEVEL = 1;
                    }
                    else if (SCORE >= 600)
                    {
                        LEVEL = 2;
                    }
                    else if (SCORE >= 1000)
                    {
                        LEVEL = 3;
                    }

                    // Redraw stage and update display
                    drawStage(console);
                    drawDISPLAYBorder(console);
                }

                // Move next block to current, generate new next
                currentBlockIndex = nextBlockIndex;
                copyBlock(currentBlock, blockModel[currentBlockIndex]);
                nextBlockIndex = rand() % 14;
                drawNextBlock(console, blockModel[nextBlockIndex]);

                blockPosX = COLUMNS / 2 - 2;
                blockPosY = 0;

                if (checkCollision(blockPosX, blockPosY, currentBlock))
                {
                    COORD pos = {10, 10};
                    SetConsoleCursorPosition(console, pos);
                    printf("=== GAME OVER ===");
                    break;
                }

                drawBlock(console, blockPosX, blockPosY, currentBlock);
            }

            lastTick = now;
        }

        Sleep(20);
    }

    return 0;
}
