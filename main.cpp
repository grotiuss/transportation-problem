#include <iostream>
#include <conio.h>
#include <iomanip>

using namespace std;

struct test {
    int index;
    test* next = NULL;
};

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

struct steppingStoneCycle {
    cell *target;
    cell *current;
    cell *cellChain = NULL;
    int level = 0;
    int score;
    bool valid = false;
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

void display_system (system_ network, string title = "Table", int iteration = 0) {
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

    cout << title;
    if (iteration > 0) {
        string iterationString = "";
        if (iteration >= 10 && iteration <= 19) {
            iterationString = to_string(iteration) + "th ";
        } else {
            switch (iteration % 10) {
                case 1:
                    iterationString = to_string(iteration) + "st ";
                    break;
                case 2:
                    iterationString = to_string(iteration) + "nd ";
                    break;
                case 3:
                    iterationString = to_string(iteration) + "rd ";
                    break;
                default:
                    iterationString = to_string(iteration) + "th ";
            }
        }
        cout << " (" << iterationString << "iteration)"; 
    }
    
    cout << endl;
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

// system_ revertDataType(steppingStoneCycl*& )

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

void rebuildingNetwork(steppingStoneCycle *&domain, system_ network, int indexTarget_i, int indexTarget_j) {
    factoryStorage *pointerFactory = network.firstFactory, *pointerStorage = network.firstStorage;
    cell *pointerCell = network.firstCell;
    int n_factories = 0, n_storages = 0;

    // counting factories and storages
    while (!(pointerFactory == NULL)) {
        n_factories ++;
        pointerFactory = pointerFactory->next;
    }
    while (!(pointerStorage == NULL)) {
        n_storages ++;
        pointerStorage = pointerStorage->next;
    }

    pointerFactory = network.firstFactory;
    pointerStorage = network.firstStorage;


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
            pointerCell = network.firstCell;
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
        pointerFactory = network.firstFactory;
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
        pointerStorage = network.firstStorage;
    }

    // connecting all cells, storages, and factories that had been created
    cell *firstCell = destination[0][0];
    factoryStorage *firstStorage = storage[0], *firstFactory = factory[0];
    for (int i = 0; i < n_factories; i++) {
        for (int j = 0; j < n_storages; j++) {
            // left
            destination[i][j]->left = (j-1) >= 0 ? destination[i][j-1] : NULL;
            //right
            destination[i][j]->right = (j+1) < n_storages ? destination[i][j+1] : NULL;
            //up
            destination[i][j]->up = (i-1) >= 0 ? destination[i-1][j] : NULL;
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

    system_ *network_ = new system_;
    network_->firstCell = firstCell;
    network_->firstFactory = firstFactory;
    network_->firstStorage = firstStorage;

    domain = new steppingStoneCycle;
    domain->target = destination[indexTarget_i][indexTarget_j];
    domain->current = destination[indexTarget_i][indexTarget_j];
    domain->valid = true;

}

void resolveSteppingStoneCellChain (steppingStoneCycle *&result) {
    int count = 0;
    cell *pHelper;
    pHelper = result->cellChain;
    while (!(pHelper == NULL)) {
        count ++;
        pHelper = pHelper->next;
    }


    cell *firstCell = result->cellChain, *lastCell = result->cellChain;
    while (lastCell->next != NULL) lastCell = lastCell->next;
    lastCell->next = firstCell; //pretend to be close cycle;

    int indexOfInflectionPoint = 0;
    cell *pointerCell[3], *inflectionPoint[count];
    pointerCell[0] = firstCell;
    pointerCell[1] = pointerCell[0]->next;
    pointerCell[2] = pointerCell[1]->next;
    while (!(pointerCell[0] == lastCell)) {
        if (pointerCell[0]->i != pointerCell[2]->i && pointerCell[0]->j != pointerCell[2]->j) {
            inflectionPoint[indexOfInflectionPoint++] = pointerCell[1];
        }
        pointerCell[0] = pointerCell[0]->next;
        pointerCell[1] = pointerCell[0]->next;
        pointerCell[2] = pointerCell[1]->next;
    }
    if (pointerCell[0]->i != pointerCell[2]->i && pointerCell[0]->j != pointerCell[2]->j) {
        inflectionPoint[indexOfInflectionPoint] = pointerCell[1];
    } else {
        indexOfInflectionPoint --;
    }
    lastCell->next = NULL;

    pHelper = result->cellChain;
    cell *reset;
    while (!(pHelper == NULL)) {
        reset = pHelper;
        pHelper = pHelper->next;
        reset->next = NULL;
    }

    for (int i = 0; i <= indexOfInflectionPoint; i++) {
        if ((i + 1) <= indexOfInflectionPoint) {
            inflectionPoint[i]->next = inflectionPoint[i + 1];
        }
    }

    inflectionPoint[indexOfInflectionPoint]->next = inflectionPoint[0]; //pretend to be close cycle;

    pHelper = inflectionPoint[0];
    while (!(pHelper->i == result->target->i && pHelper->j == result->target->j)) {
        pHelper = pHelper->next;
    }
    lastCell = pHelper;
    firstCell = lastCell->next;
    lastCell->next = NULL;
    result->cellChain = firstCell;
}

steppingStoneCycle *checkSteppingStone(steppingStoneCycle *&domain, string direction = "") {
    cell *pointerCell, *pointerCellChain, *pointerCellChain1;
    steppingStoneCycle *dummy = new steppingStoneCycle;
    steppingStoneCycle *result;

    cell *target = domain->target;
    cell *current = domain->current;
    int level = domain->level;


    string directions[] = {"up", "right", "down", "left"};
    string complementDirection = "";

    if (direction == "up") {
        complementDirection = "down";
    } else if (direction == "right") {
        complementDirection = "left";
    } else if (direction == "down") {
        complementDirection = "up";
    } else if (direction == "left") {
        complementDirection = "right";
    }

    pointerCellChain = domain->cellChain;
    if (!(pointerCellChain == NULL)) {
        while (pointerCellChain->next != NULL) {
            pointerCellChain = pointerCellChain->next;
        }
    }

    for (string direction_ : directions) {
        if (direction_ != complementDirection) {
            
            if (direction_ == "up") {
                pointerCell = current->up;
                while ((!(pointerCell == NULL)) && pointerCell->amount == 0) {
                    if ((!(pointerCell == NULL)) && (pointerCell->i == target->i && pointerCell->j == target->j)) {
                        if (!(domain->cellChain == NULL)) {
                            pointerCellChain->next = pointerCell;
                        } else {
                            domain->cellChain = pointerCell;
                        }
                        domain->current = pointerCell;
                        domain->level = level + 1;
                        return domain;
                    }
                    pointerCell = pointerCell->up;
                }
                if (!(pointerCell == NULL) && pointerCell->amount > 0) {
                    domain->current = pointerCell;
                    domain->level = level + 1;

                    if (!(domain->cellChain == NULL)) {
                        pointerCellChain->next = pointerCell;
                    } else {
                        domain->cellChain = pointerCell;
                    }

                    result = checkSteppingStone(domain, "up");
                    if (result->valid) {
                        return result;
                    } else {
                        if (domain->cellChain == pointerCell) {
                            domain->cellChain = NULL;
                        } else if (!(pointerCellChain == NULL)) {
                            pointerCellChain->next = NULL;
                        }
                    }
                }
            } else if (direction_ == "right") {
                pointerCell = current->right;
                while ((!(pointerCell == NULL)) && pointerCell->amount == 0) {
                    if ((!(pointerCell == NULL)) && (pointerCell->i == target->i && pointerCell->j == target->j)) {
                        if (!(domain->cellChain == NULL)) {
                            pointerCellChain->next = pointerCell;
                        } else {
                            domain->cellChain = pointerCell;
                        }
                        domain->current = pointerCell;
                        domain->level = level + 1;
                        return domain;
                    }
                    pointerCell = pointerCell->right;
                }
                if (!(pointerCell == NULL) && pointerCell->amount > 0) {
                    domain->current = pointerCell;
                    domain->level = level + 1;

                    if (!(domain->cellChain == NULL)) {
                        pointerCellChain->next = pointerCell;
                    } else {
                        domain->cellChain = pointerCell;
                    }

                    result = checkSteppingStone(domain, "right");

                    if (result->valid) {
                        return result;
                    } else {
                        if (domain->cellChain == pointerCell) {
                            domain->cellChain = NULL;
                        } else if (!(pointerCellChain == NULL)) {
                            pointerCellChain->next = NULL;
                        }
                    }
                }
            } else if (direction_ == "down") {
                pointerCell = current->down;
                while ((!(pointerCell == NULL)) && pointerCell->amount == 0) {
                    if ((!(pointerCell == NULL)) && (pointerCell->i == target->i && pointerCell->j == target->j)) {
                        if (!(domain->cellChain == NULL)) {
                            pointerCellChain->next = pointerCell;
                        } else {
                            domain->cellChain = pointerCell;
                        }
                        domain->current = pointerCell;
                        domain->level = level + 1;
                        return domain;
                    }
                    pointerCell = pointerCell->down;
                }
                if (!(pointerCell == NULL) && pointerCell->amount > 0) {
                    domain->current = pointerCell;
                    domain->level = level + 1;

                    if (!(domain->cellChain == NULL)) {
                        pointerCellChain->next = pointerCell;
                    } else {
                        domain->cellChain = pointerCell;
                    }

                    result = checkSteppingStone(domain, "down");
                    if (result->valid) {
                        return result;
                    } else {
                        if (domain->cellChain == pointerCell) {
                            domain->cellChain = NULL;
                        } else if (!(pointerCellChain == NULL)) {
                            pointerCellChain->next = NULL;
                        }
                    }
                }
            } else if (direction_ == "left") {
                pointerCell = current->left;
                while ((!(pointerCell == NULL)) && pointerCell->amount == 0) {
                    if ((!(pointerCell == NULL)) && (pointerCell->i == target->i && pointerCell->j == target->j)) {
                        if (!(domain->cellChain == NULL)) {
                            pointerCellChain->next = pointerCell;
                        } else {
                            domain->cellChain = pointerCell;
                        }
                        domain->current = pointerCell;
                        domain->level = level + 1;
                        return domain;
                    }
                    pointerCell = pointerCell->left;
                }
                if (!(pointerCell == NULL) && pointerCell->amount > 0) {
                    domain->current = pointerCell;
                    domain->level = level + 1;

                    if (!(domain->cellChain == NULL)) {
                        pointerCellChain->next = pointerCell;
                    } else {
                        domain->cellChain = pointerCell;
                    }

                    result = checkSteppingStone(domain, "left");
                    if (result->valid) {
                        return result;
                    } else {
                        if (domain->cellChain == pointerCell) {
                            domain->cellChain = NULL;
                        } else if (!(pointerCellChain == NULL)) {
                            pointerCellChain->next = NULL;
                        }
                    }
                }
            }
        }
    }

    return dummy;
}

system_ steppingStone (system_ network) {
    steppingStoneCycle *domain, *result;
    int indexOfCellTarget_i, indexOfCellTarget_j, minScore, score, countCellChain, countIteration = 1;
    cell *pointerCell, *pointerCellChain;

    factoryStorage *pointerFactory, *pointerStorage;
    int nFactories, nStorages;

    int amountChanges;

    do {

        // counting factories and storages
        pointerCell = network.firstCell;
        pointerFactory = network.firstFactory;
        pointerStorage = network.firstStorage;
        nFactories = 0;
        nStorages = 0;
        minScore = 0;
        while (!(pointerFactory == NULL)) {
            nFactories ++;
            pointerFactory = pointerFactory->next;
        }
        while (!(pointerStorage == NULL)) {
            nStorages ++;
            pointerStorage = pointerStorage->next;
        }

        for (int i = 0; i < nFactories; i++) {
            for (int j = 0; j < nStorages; j++) {
                while (pointerCell->i < i) {
                    pointerCell = pointerCell->down;
                }
                while (pointerCell->j < j) {
                    pointerCell = pointerCell->right;
                }

                rebuildingNetwork(domain, network, pointerCell->i, pointerCell->j);
                if (pointerCell->amount == 0) {
                    result = NULL;
                    result = checkSteppingStone(domain);
                    resolveSteppingStoneCellChain(result);
                    pointerCellChain = result->cellChain;
                    countCellChain = 0;
                    score = 0;
                    while (!(pointerCellChain == NULL)) {
                        countCellChain ++;
                        score += countCellChain % 2 == 0 ? pointerCellChain->price : ((-1)*pointerCellChain->price);
                        pointerCellChain = pointerCellChain->next;
                    }
                    if (score < minScore) {
                        minScore = score;
                        indexOfCellTarget_i = i;
                        indexOfCellTarget_j = j;
                    }
                }
                pointerCell = network.firstCell;
            }
        }

        if (minScore < 0) {


            // cout << "minScore: " << minScore << endl;

            rebuildingNetwork(domain, network, indexOfCellTarget_i, indexOfCellTarget_j);
            result = NULL;
            result = checkSteppingStone(domain);
            resolveSteppingStoneCellChain(result);

            pointerCellChain = result->cellChain;
            amountChanges = NULL; // nilai minimum dari variable keluar
            countCellChain = 0;
            while (!(pointerCellChain == NULL)) {
                countCellChain++;
                if (amountChanges) {
                    if (countCellChain % 2 != 0 && pointerCellChain->amount < amountChanges) {
                        amountChanges = pointerCellChain->amount;
                    }
                } else {
                    if (countCellChain % 2 != 0) {
                        amountChanges = pointerCellChain->amount;
                    }
                }
                pointerCellChain = pointerCellChain->next;
            }
            pointerCellChain = result->cellChain;
            countCellChain = 0;
            while(!(pointerCellChain == NULL)) {
                countCellChain++;
                pointerCellChain->amount += countCellChain % 2 == 0 ? amountChanges : ((-1)*(amountChanges));
                pointerCellChain = pointerCellChain->next;
            }

            //update network
            pointerCellChain = result->cellChain;
            while (!(pointerCellChain == NULL)) {
                
                pointerCell = network.firstCell;
                while (pointerCell->i < pointerCellChain->i) {
                    pointerCell = pointerCell->down;
                }
                while (pointerCell->j < pointerCellChain->j) {
                    pointerCell = pointerCell->right;
                }

                pointerCell->amount = pointerCellChain->amount;
                pointerCellChain = pointerCellChain->next;
            }
            network.z = zValue(network);
            cout << endl << endl;
            display_system(network, "Stepping Stone", countIteration++);
        }
        // cout << endl << "minScore: " << minScore;
    
    } while (minScore < 0);

    
    cout << endl << endl;
    cout << "finished!" << endl;

    return network;
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
    system_ network;
    cell destination[100][100];
    factoryStorage factory[100], storage[100];
    factoryStorage* currentFactory, currentStorage;
    int numberOfFactories = 0, numberOfStorages = 0, totalDemand = 0, totalSupply = 0;
    
    bool resume = true;
    while (resume) {

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
        network = steppingStone(network);

        system("pause");

        //reset
        for (int i = 0; i < numberOfFactories; i++) {
            for (int j = 0; j < numberOfStorages; j++) {
                destination[i][j].valid = false;
                destination[i][j].i = 0;
                destination[i][j].j = 0;
                destination[i][j].price = 0;
                destination[i][j].amount = 0;
                destination[i][j].up = NULL;
                destination[i][j].down = NULL;
                destination[i][j].left = NULL;
                destination[i][j].right = NULL;
                destination[i][j].next = NULL;
            }
        }
        for (int i = 0; i < numberOfFactories; i++) {
            factory[i].valid = false;
            factory[i].amount = NULL;
            factory[i].currentAmount = NULL;
            factory[i].index = NULL;
            factory[i].next = NULL;
        }
        numberOfFactories = 0;
        totalSupply = 0;
        for (int i = 0; i < numberOfStorages; i++) {
            storage[i].valid = false;
            storage[i].amount = NULL;
            storage[i].currentAmount = NULL;
            storage[i].index = NULL;
            storage[i].next = NULL;
        }
        totalDemand = 0;
        numberOfStorages = 0;
        network.firstCell = NULL;
        network.firstFactory = NULL;
        network.firstStorage = NULL;
        network.z = NULL;
    }


    return 0;
}
