#include "widget.h"
#include <QApplication>

#include "pokemon.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    Pokemon *charamander = new Pokemon();
    Pokemon *pikachu = new Pokemon();
    cout << charamander->getName() << "  " << charamander->getCurrentHP() << endl;
    pikachu->Attack(charamander);
    cout << charamander->getName() << "  " << charamander->getCurrentHP() << endl;

    return a.exec();
}
