#include <iostream>
#include <conio.h>
#include <iomanip>

using namespace std;

struct test {
    int index;
    test* next = NULL;
};

// typedef test* pointerTest;

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
    cell* next = NULL;
};

struct indexOfCell {
    int i;
    int j;
    bool valid = false;
    indexOfCell* next = NULL;
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
    int z = NULL;
};

struct cellChain {
    cell head;
    cell* next = NULL;
};

struct cycle {
    indexOfCell target;
    indexOfCell currrent;
    indexOfCell chain;
    // cellChain chain;
    int level;
    cycle* up = NULL;
    cycle* down = NULL;
    cycle* left = NULL;
    cycle* right  = NULL;
    int found = false;
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
    cout << setfill(' ') << left << setw(width) << "   Z = " + to_string(network.z);
}

int zValue (system_ network) {
    cell* pointerFirstCellOfRow = network.firstCell;
    cell* pointerCell;
    int z = 0;

    do {
        pointerCell = pointerFirstCellOfRow;
        do {
            z += (pointerCell->amount * pointerCell->price);
            pointerCell = pointerCell->right;
        } while (pointerCell != NULL);
        pointerFirstCellOfRow = pointerFirstCellOfRow->down;
    } while (pointerFirstCellOfRow != NULL);

    return z;
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

    network.z = zValue(network);
    return network;
}

// cycle cellSteppingStone (cell target, cell current, int level, cell cellChain, system_ network) {
//     cycle result, up, left, down, right;
//     cell* pointerCellChain;
//     result.target = target;
//     result.level = level;
//     result.chain = cellChain;

//     if (target.i == current.i && target.j == current.j && level > 0) {
//         if (level > 3) {
//             result.found = true;
//         }
//         return result;
//     }

//     if (cellChain.valid) {
//         pointerCellChain = &cellChain;
//         while (pointerCellChain->next != NULL) {
//             pointerCellChain = pointerCellChain->next;
//         }
//     }


//     if (current.up != NULL && (!(pointerCellChain->i == current.up->i && pointerCellChain->j == current.up->j))) {
//         if (cellChain.valid) {
//             pointerCellChain->next = current.up;
//         } else {
//             cellChain = current;
//         }
//         up = cellSteppingStone(target, *current.up, level+1, cellChain, network);
//         result.up = &up;
//     }

//     if (current.left != NULL && (!(pointerCellChain->i == current.left->i && pointerCellChain->j == current.left->j))) {
//         if (cellChain.valid) {
//             pointerCellChain->next = current.left;
//         } else {
//             cellChain = current;
//         }
//         left = cellSteppingStone(target, *current.left, level+1, cellChain, network);
//         result.left = &left;
//     }

//     if (current.down != NULL && (!(pointerCellChain->i == current.down->i && pointerCellChain->j == current.down->j))) {
//         if (cellChain.valid) {
//             pointerCellChain->next = current.down;
//         } else {
//             cellChain = current;
//         }
//         down = cellSteppingStone(target, *current.down, level+1, cellChain, network);
//         result.down = &down;
//     }

//     if (current.right != NULL && (!(pointerCellChain->i == current.right->i && pointerCellChain->j == current.right->j))) {
//         if (cellChain.valid) {
//             pointerCellChain->next = current.right;
//         } else {
//             cellChain = current;
//         }
//         right = cellSteppingStone(target, *current.right, level+1, cellChain, network);
//         result.right = &right;
//     }

//     return result;
// }

// cycle cellSteppingStone (cell target, cell current, int level, cell cellChain, system_ network) {
//     cycle result, up, left, down, right;

//     if (!cellcChain.valid) {
//         cellChain = current;
//     } else {
//         cell* pointerCellChain = cellChain;
//         while (pointerCellChain->next != NULL) {
//             pointerCellChain = pointerCellChain->next;
//         }
//         pointerCellChain->next = &current;
//     }

//     if (current.up != NULL) {
//         up = cellSteppingStone(target, *current.up, (level + 1), cellChain, network);
//     }
// }

// indexOfCell cellSteppingStone (indexOfCell target, indexOfCell current, int level, indexOfCell chain, system_ network) {
//     cell *pointerTargetCell, *pointerCurrentCell, *pointerChain;
//     // cycle result, up, down, left, rigth;

//     indexOfCell up, left, down, right;

//     pointerTargetCell = network.firstCell;
//     while (pointerTargetCell->i < target.i) {
//         pointerTargetCell = pointerTargetCell->down;
//     }
//     while (pointerTargetCell->j < target.j) {
//         pointerTargetCell = pointerTargetCell->right;
//     }

//     pointerCurrentCell = network.firstCell;
//     while (pointerCurrentCell->i < target.i) {
//         pointerCurrentCell = pointerCurrentCell->down;
//     }
//     while (pointerCurrentCell->j < target.j) {
//         pointerCurrentCell = pointerCurrentCell->right;
//     }

//     if (pointerCurrentCell->up != NULL) {
//         current.i = pointerCurrentCell->up->i;
//         current.j = pointerCurrentCell->up->j;

//         up = cellSteppingStone(target, current, (level + 1), chain, network);
//     }

// }

cell *cellSteppingStone(cell *&target, cell *&current, int level, system_ *&network) {
    cell *pointerCell = current;
    cell *dummy = new cell;
    if (current->i == target->i && current->j == target->j && level > 0) {
        cout << "level: " << level << endl;
        return current;
    }

    //left
    pointerCell = current->left;
    while (pointerCell->amount  == 0 && pointerCell != NULL) {
        pointerCell = pointerCell->left;
    }
    if (pointerCell->amount > 0 && pointerCell != NULL) {
        cout << "left" << endl;
        return cellSteppingStone(target, pointerCell, level+1, network);
    }

    //right
    pointerCell = current->right;
    while (pointerCell->amount == 0 && pointerCell != NULL) {
        pointerCell = pointerCell->right;
    }
    if (pointerCell->amount > 0 && pointerCell != NULL) {
        cout << "right";
        return cellSteppingStone(target, pointerCell, level+1, network);
    }

    //up
    pointerCell = current->up;
    while (pointerCell->amount == 0 && pointerCell != NULL) {
        pointerCell = pointerCell->up;
    }
    if (pointerCell->amount > 0 && pointerCell != NULL) {
        cout << "up";
        return cellSteppingStone(target, pointerCell, level+1, network);
    }

    //down
    pointerCell = current->down;
    while (pointerCell->amount == 0 && pointerCell != NULL) {
        pointerCell = pointerCell->down;
    }
    if (pointerCell->amount > 0 && pointerCell != NULL) {
        cout << "down";
        return cellSteppingStone(target, pointerCell, level+1, network);
    }
    
    cout << "nothing";
    

    return network->firstCell;
}

system_ steppingStone (system_ network_) {
    factoryStorage *pointerFactory = network_.firstFactory, *pointerStorage = network_.firstStorage;
    cell *pointerCell = network_.firstCell;
    int n_factories = 0, n_storages = 0;
    
    // counting factories and storages
    do {
        n_factories ++;
        pointerFactory = pointerFactory->next;
    } while (pointerFactory->next != NULL);
    do {
        n_storages ++;
        pointerStorage = pointerStorage->next;
    } while (pointerStorage->next != NULL);
    pointerFactory = network_.firstFactory;
    pointerStorage = network_.firstStorage;

    // re-building network
    cell* destination[n_factories][n_storages];
    factoryStorage *factory[n_factories], *storage[n_storages];
    for (int i = 0; i < n_factories; i++) {
        for (int j = 0; j < n_storages; j++) {
            destination[i][j] = new cell;
            while (pointerCell->i < i) {
                pointerCell = pointerCell->down;
            }
            while (pointerCell->j < j) {
                pointerCell = pointerCell->right;
            }
            destination[i][j]->valid = true;
            destination[i][j]->i = pointerCell->i;
            destination[i][j]->j = pointerCell->j;
            destination[i][j]->price = pointerCell->price;
            destination[i][j]->amount = pointerCell->amount;
            pointerCell = network_.firstCell;
        }
    }
    for (int i = 0; i < n_factories; i++) {
        factory[i] = new factoryStorage;
        while (pointerFactory->index < i) {
            pointerFactory = pointerFactory->next;
        }
        factory[i]->valid = true;
        factory[i]->amount = pointerFactory->amount;
        factory[i]->currentAmount = pointerFactory->currentAmount;
        factory[i]->index = pointerFactory->index;
        pointerFactory = network_.firstFactory;
    }
    for (int i = 0; i < n_storages; i++) {
        storage[i] = new factoryStorage;
        while (pointerStorage->index < i) {
            pointerStorage = pointerStorage->next;
        }
        storage[i]->valid = true;
        storage[i]->amount = pointerStorage->amount;
        storage[i]->currentAmount = pointerStorage->currentAmount;
        storage[i]->index = pointerStorage->index;
        pointerStorage = network_.firstStorage;
    }

    // connecting all cells, storages, and factories that had been created
    cell *firstCell = destination[0][0];
    factoryStorage *firstStorage = storage[0], *firstFactory = factory[0];
    for (int i = 0; i < n_factories; i++) {
        for (int j = 0; j < n_storages; j++) {
            // left
            destination[i][j]->left = (j-1) > 0 ? destination[i][j-1] : NULL;
            //right
            destination[i][j]->right = (j+1) < n_storages ? destination[i][j+1] : NULL;
            //up
            destination[i][j]->up = (i-1) > 0 ? destination[i-1][j] : NULL;
            //down
            destination[i][j]->down = (i+1) < n_factories ? destination[i+1][j] : NULL;
        }
    }
    for (int i = 0; i < n_factories; i++) {
        factory[i]->next = (i+1) < n_factories ? factory[i+1] : NULL;
    }
    for (int i = 0; i < n_storages; i++) {
        storage[i]->next = (i+1) < n_storages ? storage[i+1] : NULL;
    }
    
    system_ *network = new system_;
    network->firstCell = firstCell;
    network->firstFactory = firstFactory;
    network->firstStorage = firstStorage;

    cell *result = cellSteppingStone(network->firstCell->down, network->firstCell->down, 0, network);
    cout << result->i << ";" << result->j;


    return network_;
}

int function_test(test domain1, test domain2) {
    test *domain1_, *domain2_;

    domain1_ = new test;
    domain2_ = new test;

    domain1_ = &domain1;
    domain2_ = &domain2;
    domain1_->next = domain2_;

    return domain1_->next->index;
    
}

int main() {
    // test domain1, domain2;
    // domain1.index = 1;
    // domain2.index = 2;

    // cout << function_test(domain1,domain2);
    // cout << ";";
    // cout << domain1.next->index;
    // return 0;


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

    cout << endl << endl;
    network = steppingStone(network);
    return 0;
}
