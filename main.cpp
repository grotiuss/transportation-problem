#include <iostream>
#include <conio.h>
#include <iomanip>

using namespace std;

struct cell {
    bool valid = false;
    int i = 0;
    int j = 0;
    int price = 0;
    int amount = 0;
    cell* up = NULL;
    cell* down = NULL;
    cell* left = NULL;
    cell* right = NULL;
};

struct factoryStorage {
    bool valid = false;
    int amount;
    int currentAmount;
    int index;
    factoryStorage* next = NULL;
};

struct system_ {
    cell* firstCell;
    factoryStorage* firstFactory;
    factoryStorage* firstStorage;
};

void pointerUsageExample () {
    cell a, b, c, d;
    b.i = 1;
    c.i = 2;
    d.i = 3;
    a.up = &b;
    b.up = &c;
    c.up = &d;
    cout << a.up->i;
    cout << endl << a.up->up->i;
    cout << endl << a.up->up->up->i;
}

void display_system (system_ network, string title = "Table") {
    factoryStorage* currentFactory = network.firstFactory;
    factoryStorage* currentStorage = network.firstStorage;
    cell* pointerFirstCellOfRow;
    cell* pointerCell;

    int width = 15;
    int n_source = 0;
    int n_destination = 0;
    do {
        n_source ++;
        currentFactory = currentFactory->next;
    } while (currentFactory->next != NULL);
    currentFactory = network.firstFactory;
    do {
        n_destination ++;
        currentStorage = currentStorage->next;
    } while (currentStorage->next != NULL);
    currentStorage = network.firstStorage;

    cout << title << endl;
    for (int j=0; j<(n_destination + 1); j++) {
        cout << setfill('=') << left << setw(width) << "";
    }
    cout << "==";
    cout << endl;

    do {
        pointerFirstCellOfRow = network.firstCell;
        while (pointerFirstCellOfRow->i < currentFactory->index) {
            pointerFirstCellOfRow = pointerFirstCellOfRow->down;
        }

        pointerCell = pointerFirstCellOfRow;
        do {
            cout << setfill(' ') << left << setw(width) << "| " + to_string(pointerCell->price);
            pointerCell = pointerCell->right;
        } while (pointerCell!= NULL);
        // cout << setfill(' ') << left << setw(width) << "|| ";
        cout << setfill(' ') << left << setw(width) << "|| " + to_string(currentFactory->amount);
        cout << endl;
        pointerCell = pointerFirstCellOfRow;
        do {
            cout << setfill(' ') << left << setw(width) << "| " + to_string(pointerCell->amount);
            pointerCell = pointerCell->right;
        } while (pointerCell!= NULL);
        cout << setfill(' ') << left << setw(width) << "|| ";
        cout << endl;
        pointerCell = pointerFirstCellOfRow;
        do {
            cout << setfill('-') << left << setw(width) << "";
            pointerCell = pointerCell->right;
        } while (pointerCell!= NULL);
        cout << "--";
        cout << endl;

        currentFactory = currentFactory->next;
    } while (currentFactory != NULL);

    do {
        cout << setfill(' ') << left << setw(width) << "  " + to_string(currentStorage->amount);
        currentStorage = currentStorage->next;
    } while (currentStorage != NULL);

}

system_ northWestCorner (system_ network) {
    factoryStorage* currentFactory = network.firstFactory;
    factoryStorage* currentStorage = network.firstStorage;
    cell* pointerFirstCellOfRow;
    cell* pointerCell;
    int indexCellRow, indexCellColumn;

    do {
        indexCellRow = currentFactory->index;
        indexCellColumn = currentStorage->index;

        pointerFirstCellOfRow = network.firstCell;
        while (pointerFirstCellOfRow->i < indexCellRow && pointerFirstCellOfRow->down != NULL) {
            pointerFirstCellOfRow = pointerFirstCellOfRow->down;
        }
        if (pointerFirstCellOfRow->i != indexCellRow) {
            system("CLS");
            cout << "Internal error";
        }
        pointerCell = pointerFirstCellOfRow;
        while (pointerCell->j < indexCellColumn && pointerCell->right != NULL) {
            pointerCell = pointerCell->right;
        }
        if (pointerCell->j != indexCellColumn) {
            system("CLS");
            cout << "Internal error";
        }

        if (currentFactory->currentAmount < currentStorage->currentAmount) {
            pointerCell->amount = currentFactory->currentAmount;
            currentStorage->currentAmount -= currentFactory->currentAmount;
            currentFactory->currentAmount = 0;
            currentFactory = currentFactory->next;
        } else if (currentFactory->currentAmount > currentStorage->currentAmount) {
            pointerCell->amount = currentStorage->currentAmount;
            currentFactory->currentAmount -= currentStorage->currentAmount;
            currentStorage->currentAmount = 0;
            currentStorage = currentStorage->next;
        } else {
            pointerCell->amount = currentFactory->currentAmount;
            currentFactory->currentAmount = 0;
            currentStorage->currentAmount = 0;
            currentFactory = currentFactory->next;
            currentStorage = currentStorage->next;
        }
    } while (currentFactory != NULL && currentStorage != NULL);

    return network;
}

int main() {
    system_ network;
    cell destination[100][100];
    factoryStorage factory[100], storage[100];
    factoryStorage* currentFactory, currentStorage;
    int numberOfFactories, numberOfStorages, totalDemand = 0, totalSupply = 0;

    cout << "Number of Factories: "; cin >> numberOfFactories;
    cout << "Number of Storages: " ; cin >> numberOfStorages;

    //input amount of supply for each factory
    for (int i = 0; i < numberOfFactories; i++) {
        cout << "Factory - " << i << ": " ; cin >> factory[i].amount;
        factory[i].currentAmount = factory[i].amount;
        factory[i].valid = true;
        factory[i].index = i;
        totalSupply += factory[i].amount;
        if ((i-1 >= 0) && factory[i-1].valid) {
            factory[i-1].next = &factory[i];
        }
        if (i == 0) {
            network.firstFactory = &factory[i];
        }
    }
    //input amount of demand for each storage
    for (int i = 0; i < numberOfStorages; i++) {
        cout << "Storage - " << i << ": " ; cin >> storage[i].amount;
        storage[i].currentAmount = storage[i].amount;
        storage[i].valid = true;
        storage[i].index = i;
        totalDemand += storage[i].amount;
        if ((i-1 >= 0) && storage[i-1].valid) {
            storage[i-1].next = &storage[i];
        }
        if (i == 0) {
            network.firstStorage = &storage[i];
        }
    }
    //totalDemand and totalSupply must be equal (balance)
    if (totalDemand != totalSupply) {
        cout << "total demand and total supply must be equal or balance";
        return 0;
    }
    //input price for each cell
    for (int i = 0; i < numberOfFactories; i++) {
        for (int j = 0; j < numberOfStorages; j++) {
            destination[i][j].i = i;
            destination[i][j].j = j;
            destination[i][j].valid = true;
            cout << "Factory " << i << " - Storage " << j << ": "; cin >> destination[i][j].price;
            if (i==0 && j==0) {
                network.firstCell = &destination[i][j];
            }
        }
    }
    //Connecting all cells
    for (int i = 0; i < numberOfFactories; i++) {
        for (int j = 0; j < numberOfStorages; j++) {
            // up
            if ((i-1 >= 0) && destination[i-1][j].valid) {
                destination[i][j].up = &destination[i-1][j];
            }
            // down
            if ((i+1 < numberOfFactories) && destination[i+1][j].valid) {
                destination[i][j].down = &destination[i+1][j];
            }
            //left
            if ((j-1 >= 0) && destination[i][j-1].valid) {
                destination[i][j].left = &destination[i][j-1];
            }
            //right
            if ((j+1 < numberOfStorages) && destination[i][j+1].valid) {
                destination[i][j].right = &destination[i][j+1];
            }
        }
    }

    network = northWestCorner(network);
    cout << endl << endl;

    display_system(network, "North-West Corner");

    return 0;
}
