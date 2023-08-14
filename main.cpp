#include <fstream>

#include <QApplication>

#include "MainWindow.h"
#include "Global.h"

using namespace std;

void checkLocalFile() {
    ifstream ifs(FACE_DATABASE_PATH);

    if (!ifs.good()) {
        ofstream ofs(FACE_DATABASE_PATH);
        ofs << "name,";
        for (int i = 0; i < 128; ++i) {
            string s = "d";
            s += to_string(i);
            if (i != 127) ofs << s << ',';
            else ofs << s;
        }
        ofs << endl;
        ofs.close();
    }
}

int main(int argc, char *argv[]) {
    checkLocalFile();

    QApplication a(argc, argv);

    auto* mainWindow = new MainWindow(nullptr);
    mainWindow->show();

    return QApplication::exec();
}
