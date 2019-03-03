// THIS CONTAINS MAIN FUNCTION -- BE WEARY MON
#include "main.h"
#include <vector>
using namespace std;
void simple_text_menu();



auto main() -> int
{main1();
 //simple_text_menu();
}

// void test()

void simple_text_menu()
{  // TODO : include from the bloddy cmake variables
  int n{0};
  cout << "Please choose a option to display" << endl;
  cout << "0. EXIT (0)" << endl;
  cout << "1. Part1" << endl;
  //  main1();

  cin >> n;
  switch ( n )
  {
    case 1:
      one();
      simple_text_menu();
      break;  // will never be executed still no warning
    case 2:
      main1();
      simple_text_menu();
      break;
    case 0:
      exit( 0 );
    default:
      cout << "are u sure\n";
      simple_text_menu();
  }
}
