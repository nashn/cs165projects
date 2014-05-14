#include <iostream>
#include <string>

using namespace std;
int main(int argc, char *argv[]) {

        int count[256];

        for (int i = 0; i < 256; i++)
        {
                count[i] = 0;
        }

        string line = "";
        cout << "Enter a string > \n";

        getline(cin, line);

        for (int i = 0; i < line.size(); i++) {
                if ( line[i] == '\0' ) {
                        break;
                }
                count[(int)line[i]]++;
        }

        for ( int i = 0; i < 256; i++) {
                if ( count[i] != 0 )
                        cout << (char)i << " occurs " << count[i] << " times in the entered string." << endl;
        }

        return 0;
}
