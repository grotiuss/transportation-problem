#include <iostream>
#include <conio.h>
#include <iomanip>

using namespace std;

struct shipment {
    bool valid = false;
    int i = 0;
    int j = 0;
    int price = 0;
    int amountOfResources = 0;
    shipment* up = NULL;
    shipment* down = NULL;
    shipment* left = NULL;
    shipment* right = NULL;
    shipment* next = NULL;
};

struct supplierDestination {
    bool valid = false;
    int amountOfResources;
    int currentAmountOfResources;
    int index;
    supplierDestination* next = NULL;
};

struct transportationProblem {
    shipment* firstShipment;
    supplierDestination* firstSupplier;
    supplierDestination* firstDestination;
    int z = NULL;
};

struct steppingStoneCycle {
    shipment *target;
    shipment *current;
    shipment *cycle = NULL;
    int level = 0;
    int score;
    bool valid = false;
};

void display_system (transportationProblem network, string title = "Table", int iteration = 0) {
    supplierDestination* currentSupplier = network.firstSupplier;
    supplierDestination* currentDestination = network.firstDestination;
    shipment* pointerFirstShipmentOfRow;
    shipment* pointerShipment;

    int width = 15;
    int n_source = 0;
    int n_destination = 0;
    do {
        n_source ++;
        currentSupplier = currentSupplier->next;
    } while (currentSupplier->next != NULL);
    currentSupplier = network.firstSupplier;
    do {
        n_destination ++;
        currentDestination = currentDestination->next;
    } while (currentDestination->next != NULL);
    currentDestination = network.firstDestination;

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
        pointerFirstShipmentOfRow = network.firstShipment;
        while (pointerFirstShipmentOfRow->i < currentSupplier->index) {
            pointerFirstShipmentOfRow = pointerFirstShipmentOfRow->down;
        }

        pointerShipment = pointerFirstShipmentOfRow;
        do {
            cout << setfill(' ') << left << setw(width) << "| " + to_string(pointerShipment->price);
            pointerShipment = pointerShipment->right;
        } while (pointerShipment!= NULL);

        cout << setfill(' ') << left << setw(width) << "|| " + to_string(currentSupplier->amountOfResources);
        cout << endl;
        pointerShipment = pointerFirstShipmentOfRow;
        do {
            cout << setfill(' ') << left << setw(width) << "| " + to_string(pointerShipment->amountOfResources);
            pointerShipment = pointerShipment->right;
        } while (pointerShipment!= NULL);
        cout << setfill(' ') << left << setw(width) << "|| ";
        cout << endl;
        pointerShipment = pointerFirstShipmentOfRow;
        do {
            cout << setfill('-') << left << setw(width) << "";
            pointerShipment = pointerShipment->right;
        } while (pointerShipment!= NULL);
        cout << "--";
        cout << endl;

        currentSupplier = currentSupplier->next;
    } while (currentSupplier != NULL);

    do {
        cout << setfill(' ') << left << setw(width) << "  " + to_string(currentDestination->amountOfResources);
        currentDestination = currentDestination->next;
    } while (currentDestination != NULL);
    cout << setfill(' ') << left << setw(width) << "   Z = " + to_string(network.z);
}

int zValue (transportationProblem network) {
    shipment* pointerFirstShipmentOfRow = network.firstShipment;
    shipment* pointerShipment;
    int z = 0;

    do {
        pointerShipment = pointerFirstShipmentOfRow;
        do {
            z += (pointerShipment->amountOfResources * pointerShipment->price);
            pointerShipment = pointerShipment->right;
        } while (pointerShipment != NULL);
        pointerFirstShipmentOfRow = pointerFirstShipmentOfRow->down;
    } while (pointerFirstShipmentOfRow != NULL);

    return z;
}

transportationProblem northWestCorner (transportationProblem network) {
    supplierDestination* currentSupplier = network.firstSupplier;
    supplierDestination* currentDestination = network.firstDestination;
    shipment* pointerFirstShipmentOfRow;
    shipment* pointerShipment;
    int indexShipmentRow, indexShipmentColumn;

    do {
        indexShipmentRow = currentSupplier->index;
        indexShipmentColumn = currentDestination->index;

        pointerFirstShipmentOfRow = network.firstShipment;
        while (pointerFirstShipmentOfRow->i < indexShipmentRow && pointerFirstShipmentOfRow->down != NULL) {
            pointerFirstShipmentOfRow = pointerFirstShipmentOfRow->down;
        }
        if (pointerFirstShipmentOfRow->i != indexShipmentRow) {
            system("CLS");
            cout << "Internal error";
        }
        pointerShipment = pointerFirstShipmentOfRow;
        while (pointerShipment->j < indexShipmentColumn && pointerShipment->right != NULL) {
            pointerShipment = pointerShipment->right;
        }
        if (pointerShipment->j != indexShipmentColumn) {
            system("CLS");
            cout << "Internal error";
        }

        if (currentSupplier->currentAmountOfResources < currentDestination->currentAmountOfResources) {
            pointerShipment->amountOfResources = currentSupplier->currentAmountOfResources;
            currentDestination->currentAmountOfResources -= currentSupplier->currentAmountOfResources;
            currentSupplier->currentAmountOfResources = 0;
            currentSupplier = currentSupplier->next;
        } else if (currentSupplier->currentAmountOfResources > currentDestination->currentAmountOfResources) {
            pointerShipment->amountOfResources = currentDestination->currentAmountOfResources;
            currentSupplier->currentAmountOfResources -= currentDestination->currentAmountOfResources;
            currentDestination->currentAmountOfResources = 0;
            currentDestination = currentDestination->next;
        } else {
            pointerShipment->amountOfResources = currentSupplier->currentAmountOfResources;
            currentSupplier->currentAmountOfResources = 0;
            currentDestination->currentAmountOfResources = 0;
            currentSupplier = currentSupplier->next;
            currentDestination = currentDestination->next;
        }
    } while (currentSupplier != NULL && currentDestination != NULL);

    network.z = zValue(network);
    return network;
}

void rebuildingNetwork(steppingStoneCycle *&domain, transportationProblem network, int indexTarget_i, int indexTarget_j) {
    supplierDestination *pointerSupplier = network.firstSupplier, *pointerDestination = network.firstDestination;
    shipment *pointerShipment = network.firstShipment;
    int nSupplier = 0, nDestination = 0;

    // counting factories and storages
    while (!(pointerSupplier == NULL)) {
        nSupplier ++;
        pointerSupplier = pointerSupplier->next;
    }
    while (!(pointerDestination == NULL)) {
        nDestination ++;
        pointerDestination = pointerDestination->next;
    }

    pointerSupplier = network.firstSupplier;
    pointerDestination = network.firstDestination;


    // re-building network
    shipment* shipmentData[nSupplier][nDestination];
    supplierDestination *supplier[nSupplier], *destination[nDestination];
    for (int i = 0; i < nSupplier; i++) {
        for (int j = 0; j < nDestination; j++) {
            shipmentData[i][j] = new shipment;
            while (pointerShipment->i < i) {
                pointerShipment = pointerShipment->down;
            }
            while (pointerShipment->j < j) {
                pointerShipment = pointerShipment->right;
            }
            shipmentData[i][j]->valid = true;
            shipmentData[i][j]->i = pointerShipment->i;
            shipmentData[i][j]->j = pointerShipment->j;
            shipmentData[i][j]->price = pointerShipment->price;
            shipmentData[i][j]->amountOfResources = pointerShipment->amountOfResources;
            pointerShipment = network.firstShipment;
        }
    }
    for (int i = 0; i < nSupplier; i++) {
        supplier[i] = new supplierDestination;
        while (pointerSupplier->index < i) {
            pointerSupplier = pointerSupplier->next;
        }
        supplier[i]->valid = true;
        supplier[i]->amountOfResources = pointerSupplier->amountOfResources;
        supplier[i]->currentAmountOfResources = pointerSupplier->currentAmountOfResources;
        supplier[i]->index = pointerSupplier->index;
        pointerSupplier = network.firstSupplier;
    }
    for (int i = 0; i < nDestination; i++) {
        destination[i] = new supplierDestination;
        while (pointerDestination->index < i) {
            pointerDestination = pointerDestination->next;
        }
        destination[i]->valid = true;
        destination[i]->amountOfResources = pointerDestination->amountOfResources;
        destination[i]->currentAmountOfResources = pointerDestination->currentAmountOfResources;
        destination[i]->index = pointerDestination->index;
        pointerDestination = network.firstDestination;
    }

    // connecting all cells, storages, and factories that had been created
    shipment *firstShipment = shipmentData[0][0];
    supplierDestination *firstDestination = destination[0], *firstSupplier = supplier[0];
    for (int i = 0; i < nSupplier; i++) {
        for (int j = 0; j < nDestination; j++) {
            // left
            shipmentData[i][j]->left = (j-1) >= 0 ? shipmentData[i][j-1] : NULL;
            //right
            shipmentData[i][j]->right = (j+1) < nDestination ? shipmentData[i][j+1] : NULL;
            //up
            shipmentData[i][j]->up = (i-1) >= 0 ? shipmentData[i-1][j] : NULL;
            //down
            shipmentData[i][j]->down = (i+1) < nSupplier ? shipmentData[i+1][j] : NULL;
        }
    }
    for (int i = 0; i < nSupplier; i++) {
        supplier[i]->next = (i+1) < nSupplier ? supplier[i+1] : NULL;
    }
    for (int i = 0; i < nDestination; i++) {
        destination[i]->next = (i+1) < nDestination ? destination[i+1] : NULL;
    }

    transportationProblem *network_ = new transportationProblem;
    network_->firstShipment = firstShipment;
    network_->firstSupplier = firstSupplier;
    network_->firstDestination = firstDestination;

    domain = new steppingStoneCycle;
    domain->target = shipmentData[indexTarget_i][indexTarget_j];
    domain->current = shipmentData[indexTarget_i][indexTarget_j];
    domain->valid = true;

}

void resolveSteppingStoneCellChain (steppingStoneCycle *&result) {
    int count = 0;
    shipment *pHelper;
    pHelper = result->cycle;
    while (!(pHelper == NULL)) {
        count ++;
        pHelper = pHelper->next;
    }


    shipment *firstShipment = result->cycle, *lastShipment = result->cycle;
    while (lastShipment->next != NULL) lastShipment = lastShipment->next;
    lastShipment->next = firstShipment; //pretend to be close cycle;

    int indexOfInflectionPoint = 0;
    shipment *pointerShipment[3], *inflectionPoint[count];
    pointerShipment[0] = firstShipment;
    pointerShipment[1] = pointerShipment[0]->next;
    pointerShipment[2] = pointerShipment[1]->next;
    while (!(pointerShipment[0] == lastShipment)) {
        if (pointerShipment[0]->i != pointerShipment[2]->i && pointerShipment[0]->j != pointerShipment[2]->j) {
            inflectionPoint[indexOfInflectionPoint++] = pointerShipment[1];
        }
        pointerShipment[0] = pointerShipment[0]->next;
        pointerShipment[1] = pointerShipment[0]->next;
        pointerShipment[2] = pointerShipment[1]->next;
    }
    if (pointerShipment[0]->i != pointerShipment[2]->i && pointerShipment[0]->j != pointerShipment[2]->j) {
        inflectionPoint[indexOfInflectionPoint] = pointerShipment[1];
    } else {
        indexOfInflectionPoint --;
    }
    lastShipment->next = NULL;

    pHelper = result->cycle;
    shipment *reset;
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
    lastShipment = pHelper;
    firstShipment = lastShipment->next;
    lastShipment->next = NULL;
    result->cycle = firstShipment;
}

steppingStoneCycle *checkSteppingStone(steppingStoneCycle *&domain, string direction = "") {
    shipment *pointerShipment, *pointerShipmentCycle;
    steppingStoneCycle *dummy = new steppingStoneCycle;
    steppingStoneCycle *result;

    shipment *target = domain->target;
    shipment *current = domain->current;
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

    pointerShipmentCycle = domain->cycle;
    if (!(pointerShipmentCycle == NULL)) {
        while (pointerShipmentCycle->next != NULL) {
            pointerShipmentCycle = pointerShipmentCycle->next;
        }
    }

    for (string direction_ : directions) {
        if (direction_ != complementDirection) {
            
            if (direction_ == "up") {
                pointerShipment = current->up;
                while ((!(pointerShipment == NULL)) && pointerShipment->amountOfResources == 0) {
                    if ((!(pointerShipment == NULL)) && (pointerShipment->i == target->i && pointerShipment->j == target->j)) {
                        if (!(domain->cycle == NULL)) {
                            pointerShipmentCycle->next = pointerShipment;
                        } else {
                            domain->cycle = pointerShipment;
                        }
                        domain->current = pointerShipment;
                        domain->level = level + 1;
                        return domain;
                    }
                    pointerShipment = pointerShipment->up;
                }
                if (!(pointerShipment == NULL) && pointerShipment->amountOfResources > 0) {
                    domain->current = pointerShipment;
                    domain->level = level + 1;

                    if (!(domain->cycle == NULL)) {
                        pointerShipmentCycle->next = pointerShipment;
                    } else {
                        domain->cycle = pointerShipment;
                    }

                    result = checkSteppingStone(domain, "up");
                    if (result->valid) {
                        return result;
                    } else {
                        if (domain->cycle == pointerShipment) {
                            domain->cycle = NULL;
                        } else if (!(pointerShipmentCycle == NULL)) {
                            pointerShipmentCycle->next = NULL;
                        }
                    }
                }
            } else if (direction_ == "right") {
                pointerShipment = current->right;
                while ((!(pointerShipment == NULL)) && pointerShipment->amountOfResources == 0) {
                    if ((!(pointerShipment == NULL)) && (pointerShipment->i == target->i && pointerShipment->j == target->j)) {
                        if (!(domain->cycle == NULL)) {
                            pointerShipmentCycle->next = pointerShipment;
                        } else {
                            domain->cycle = pointerShipment;
                        }
                        domain->current = pointerShipment;
                        domain->level = level + 1;
                        return domain;
                    }
                    pointerShipment = pointerShipment->right;
                }
                if (!(pointerShipment == NULL) && pointerShipment->amountOfResources > 0) {
                    domain->current = pointerShipment;
                    domain->level = level + 1;

                    if (!(domain->cycle == NULL)) {
                        pointerShipmentCycle->next = pointerShipment;
                    } else {
                        domain->cycle = pointerShipment;
                    }

                    result = checkSteppingStone(domain, "right");

                    if (result->valid) {
                        return result;
                    } else {
                        if (domain->cycle == pointerShipment) {
                            domain->cycle = NULL;
                        } else if (!(pointerShipmentCycle == NULL)) {
                            pointerShipmentCycle->next = NULL;
                        }
                    }
                }
            } else if (direction_ == "down") {
                pointerShipment = current->down;
                while ((!(pointerShipment == NULL)) && pointerShipment->amountOfResources == 0) {
                    if ((!(pointerShipment == NULL)) && (pointerShipment->i == target->i && pointerShipment->j == target->j)) {
                        if (!(domain->cycle == NULL)) {
                            pointerShipmentCycle->next = pointerShipment;
                        } else {
                            domain->cycle = pointerShipment;
                        }
                        domain->current = pointerShipment;
                        domain->level = level + 1;
                        return domain;
                    }
                    pointerShipment = pointerShipment->down;
                }
                if (!(pointerShipment == NULL) && pointerShipment->amountOfResources > 0) {
                    domain->current = pointerShipment;
                    domain->level = level + 1;

                    if (!(domain->cycle == NULL)) {
                        pointerShipmentCycle->next = pointerShipment;
                    } else {
                        domain->cycle = pointerShipment;
                    }

                    result = checkSteppingStone(domain, "down");
                    if (result->valid) {
                        return result;
                    } else {
                        if (domain->cycle == pointerShipment) {
                            domain->cycle = NULL;
                        } else if (!(pointerShipmentCycle == NULL)) {
                            pointerShipmentCycle->next = NULL;
                        }
                    }
                }
            } else if (direction_ == "left") {
                pointerShipment = current->left;
                while ((!(pointerShipment == NULL)) && pointerShipment->amountOfResources == 0) {
                    if ((!(pointerShipment == NULL)) && (pointerShipment->i == target->i && pointerShipment->j == target->j)) {
                        if (!(domain->cycle == NULL)) {
                            pointerShipmentCycle->next = pointerShipment;
                        } else {
                            domain->cycle = pointerShipment;
                        }
                        domain->current = pointerShipment;
                        domain->level = level + 1;
                        return domain;
                    }
                    pointerShipment = pointerShipment->left;
                }
                if (!(pointerShipment == NULL) && pointerShipment->amountOfResources > 0) {
                    domain->current = pointerShipment;
                    domain->level = level + 1;

                    if (!(domain->cycle == NULL)) {
                        pointerShipmentCycle->next = pointerShipment;
                    } else {
                        domain->cycle = pointerShipment;
                    }

                    result = checkSteppingStone(domain, "left");
                    if (result->valid) {
                        return result;
                    } else {
                        if (domain->cycle == pointerShipment) {
                            domain->cycle = NULL;
                        } else if (!(pointerShipmentCycle == NULL)) {
                            pointerShipmentCycle->next = NULL;
                        }
                    }
                }
            }
        }
    }

    return dummy;
}

transportationProblem steppingStone (transportationProblem network) {
    steppingStoneCycle *domain, *result;
    int indexOfCellTarget_i, indexOfCellTarget_j, minScore, score, countCellChain, countIteration = 1;
    shipment *pointerShipment, *pointerShipmentCycle;

    supplierDestination *pointerFactory, *pointerStorage;
    int nFactories, nStorages;

    int amountChanges;

    do {
        // counting factories and storages
        pointerShipment = network.firstShipment;
        pointerFactory = network.firstSupplier;
        pointerStorage = network.firstDestination;
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
                while (pointerShipment->i < i) {
                    pointerShipment = pointerShipment->down;
                }
                while (pointerShipment->j < j) {
                    pointerShipment = pointerShipment->right;
                }

                rebuildingNetwork(domain, network, pointerShipment->i, pointerShipment->j);
                if (pointerShipment->amountOfResources == 0) {
                    result = NULL;
                    result = checkSteppingStone(domain);
                    resolveSteppingStoneCellChain(result);
                    pointerShipmentCycle = result->cycle;
                    countCellChain = 0;
                    score = 0;
                    while (!(pointerShipmentCycle == NULL)) {
                        countCellChain ++;
                        score += countCellChain % 2 == 0 ? pointerShipmentCycle->price : ((-1)*pointerShipmentCycle->price);
                        pointerShipmentCycle = pointerShipmentCycle->next;
                    }
                    if (score < minScore) {
                        minScore = score;
                        indexOfCellTarget_i = i;
                        indexOfCellTarget_j = j;
                    }
                }
                pointerShipment = network.firstShipment;
            }
        }

        if (minScore < 0) {
            rebuildingNetwork(domain, network, indexOfCellTarget_i, indexOfCellTarget_j);
            result = NULL;
            result = checkSteppingStone(domain);
            resolveSteppingStoneCellChain(result);

            pointerShipmentCycle = result->cycle;
            amountChanges = NULL; // nilai minimum dari variable keluar
            countCellChain = 0;
            while (!(pointerShipmentCycle == NULL)) {
                countCellChain++;
                if (amountChanges) {
                    if (countCellChain % 2 != 0 && pointerShipmentCycle->amountOfResources < amountChanges) {
                        amountChanges = pointerShipmentCycle->amountOfResources;
                    }
                } else {
                    if (countCellChain % 2 != 0) {
                        amountChanges = pointerShipmentCycle->amountOfResources;
                    }
                }
                pointerShipmentCycle = pointerShipmentCycle->next;
            }
            pointerShipmentCycle = result->cycle;
            countCellChain = 0;
            while(!(pointerShipmentCycle == NULL)) {
                countCellChain++;
                pointerShipmentCycle->amountOfResources += countCellChain % 2 == 0 ? amountChanges : ((-1)*(amountChanges));
                pointerShipmentCycle = pointerShipmentCycle->next;
            }

            //update network
            pointerShipmentCycle = result->cycle;
            while (!(pointerShipmentCycle == NULL)) {
                
                pointerShipment = network.firstShipment;
                while (pointerShipment->i < pointerShipmentCycle->i) {
                    pointerShipment = pointerShipment->down;
                }
                while (pointerShipment->j < pointerShipmentCycle->j) {
                    pointerShipment = pointerShipment->right;
                }

                pointerShipment->amountOfResources = pointerShipmentCycle->amountOfResources;
                pointerShipmentCycle = pointerShipmentCycle->next;
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

int main() {
    transportationProblem network;
    shipment destination[100][100];
    supplierDestination factory[100], storage[100];
    supplierDestination* currentFactory, currentStorage;
    int numberOfFactories = 0, numberOfStorages = 0, totalDemand = 0, totalSupply = 0;
    
    bool resume = true;
    while (resume) {

        cout << "Number of Factories: "; cin >> numberOfFactories;
        cout << "Number of Storages: " ; cin >> numberOfStorages;

        //input amountOfResources of supply for each factory
        for (int i = 0; i < numberOfFactories; i++) {
            cout << "Factory - " << (i+1) << ": " ; cin >> factory[i].amountOfResources;
            factory[i].currentAmountOfResources = factory[i].amountOfResources;
            factory[i].valid = true;
            factory[i].index = i;
            totalSupply += factory[i].amountOfResources;
            if ((i-1 >= 0) && factory[i-1].valid) {
                factory[i-1].next = &factory[i];
            }
            if (i == 0) {
                network.firstSupplier = &factory[i];
            }
        }
        //input amountOfResources of demand for each storage
        for (int i = 0; i < numberOfStorages; i++) {
            cout << "Storage - " << (i+1) << ": " ; cin >> storage[i].amountOfResources;
            storage[i].currentAmountOfResources = storage[i].amountOfResources;
            storage[i].valid = true;
            storage[i].index = i;
            totalDemand += storage[i].amountOfResources;
            if ((i-1 >= 0) && storage[i-1].valid) {
                storage[i-1].next = &storage[i];
            }
            if (i == 0) {
                network.firstDestination = &storage[i];
            }
        }
        //totalDemand and totalSupply must be equal (balance)
        if (totalDemand != totalSupply) {
            cout << "total demand and total supply must be equal or balance" << endl << endl;
        } else {
            //input price for each shipment
            for (int i = 0; i < numberOfFactories; i++) {
                for (int j = 0; j < numberOfStorages; j++) {
                    destination[i][j].i = i;
                    destination[i][j].j = j;
                    destination[i][j].valid = true;
                    cout << "Factory " << (i+1) << " - Storage " << (j+1) << ": "; cin >> destination[i][j].price;
                    if (i==0 && j==0) {
                        network.firstShipment = &destination[i][j];
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
        }

        system("pause");

        //reset
        for (int i = 0; i < numberOfFactories; i++) {
            for (int j = 0; j < numberOfStorages; j++) {
                destination[i][j].valid = false;
                destination[i][j].i = 0;
                destination[i][j].j = 0;
                destination[i][j].price = 0;
                destination[i][j].amountOfResources = 0;
                destination[i][j].up = NULL;
                destination[i][j].down = NULL;
                destination[i][j].left = NULL;
                destination[i][j].right = NULL;
                destination[i][j].next = NULL;
            }
        }
        for (int i = 0; i < numberOfFactories; i++) {
            factory[i].valid = false;
            factory[i].amountOfResources = NULL;
            factory[i].currentAmountOfResources = NULL;
            factory[i].index = NULL;
            factory[i].next = NULL;
        }
        numberOfFactories = 0;
        totalSupply = 0;
        for (int i = 0; i < numberOfStorages; i++) {
            storage[i].valid = false;
            storage[i].amountOfResources = NULL;
            storage[i].currentAmountOfResources = NULL;
            storage[i].index = NULL;
            storage[i].next = NULL;
        }
        totalDemand = 0;
        numberOfStorages = 0;
        network.firstShipment = NULL;
        network.firstSupplier = NULL;
        network.firstDestination = NULL;
        network.z = NULL;

        system("CLS");
    }


    return 0;
}