#include <iostream>

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
    factoryStorage* next = NULL;
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

int main() {
    cell destination[100][100];
    factoryStorage factory[100], storage[100];
    int numberOfFactories, numberOfStorages, totalDemand = 0, totalSupply = 0;

    cout << "Number of Factories: "; cin >> numberOfFactories;
    cout << "Number of Storages: " ; cin >> numberOfStorages;

    //input amount of supply for each factory
    for (int i = 0; i < numberOfFactories; i++) {
        cout << "Factory - " << i << ": " ; cin >> factory[i].amount;
        factory[i].currentAmount = factory[i].amount;
        factory[i].valid = true;
        totalSupply += factory[i].amount;
    }
    //input amount of demand for each storage
    for (int i = 0; i < numberOfStorages; i++) {
        cout << "Storage - " << i << ": " ; cin >> storage[i].amount;
        storage[i].currentAmount = storage[i].amount;
        storage[i].valid = true;
        totalDemand += storage[i].amount;
        if ((i-1 >= 0) && storage[i-1].valid) {
            storage[i-1].next = &storage[i];
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



    return 0;
}
