#include <ncurses.h>
#include <iostream>

int main()
{
  // setlocale(LC_ALL, ""); // 设置字符集为UTF-8
  // 初始化 ncurses
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  // 打印提示信息
  printw("please input key (press 'q' to exit)...\n");
  refresh();

  // 循环等待键盘事件
  int cnt = 0;
  while (1)
  {
    int ch = getch(); // 获取按下的键值

    if (ch == 'q' || ch == 'Q')
      break;

    // 清除之前的提示信息
    clear();

    // 显示按下的键值
    printw("you press key: %c, cnt: %d\n", ch, cnt);
    ++cnt;
    refresh();
  }

  // 清理 ncurses
  endwin();

  return 0;
}
