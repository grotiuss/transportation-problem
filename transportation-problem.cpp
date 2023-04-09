#include <iostream>
#include <conio.h>
#include <iomanip>

using namespace std;

struct shipment {
    bool valid = false;
    int i = 0;
    int j = 0;
    int c = 0;
    int x = 0;
    shipment* up = NULL;
    shipment* down = NULL;
    shipment* left = NULL;
    shipment* right = NULL;
    shipment* next = NULL;
};

struct supplierDestination {
    bool valid = false;
    int a;
    int aTemp; // 'a' value that will be changed while finding an initial solution
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
    bool solved = false;
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
            cout << setfill(' ') << left << setw(width) << "| " + to_string(pointerShipment->c);
            pointerShipment = pointerShipment->right;
        } while (pointerShipment!= NULL);

        cout << setfill(' ') << left << setw(width) << "|| " + to_string(currentSupplier->a);
        cout << endl;
        pointerShipment = pointerFirstShipmentOfRow;
        do {
            cout << setfill(' ') << left << setw(width) << "| " + to_string(pointerShipment->x);
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
        cout << setfill(' ') << left << setw(width) << "  " + to_string(currentDestination->a);
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
            z += (pointerShipment->x * pointerShipment->c);
            pointerShipment = pointerShipment->right;
        } while (pointerShipment != NULL);
        pointerFirstShipmentOfRow = pointerFirstShipmentOfRow->down;
    } while (pointerFirstShipmentOfRow != NULL);

    return z;
}

transportationProblem northWestCorner (transportationProblem network) {
    supplierDestination *pSupplier = network.firstSupplier;
    supplierDestination *pDestination = network.firstDestination;
    shipment *pShipment;

    do {
        pShipment = network.firstShipment;
        while (pShipment->i < pSupplier->index && pShipment->down != NULL) {
            pShipment = pShipment->down;
        }
        while (pShipment->j < pDestination->index && pShipment->right != NULL) {
            pShipment = pShipment->right;
        }

        if (pSupplier->aTemp < pDestination->aTemp) {
            pShipment->x = pSupplier->aTemp;
            pDestination->aTemp -= pSupplier->aTemp;
            pSupplier->aTemp = 0;
            pSupplier = pSupplier->next;
        } else if (pSupplier->aTemp > pDestination->aTemp) {
            pShipment->x = pDestination->aTemp;
            pSupplier->aTemp -= pDestination->aTemp;
            pDestination->aTemp = 0;
            pDestination = pDestination->next;
        } else {
            pShipment->x = pSupplier->aTemp;
            pSupplier->aTemp = 0;
            pDestination->aTemp = 0;
            pSupplier = pSupplier->next;
            pDestination = pDestination->next;
        }
    } while (pSupplier != NULL && pDestination != NULL);

    network.z = zValue(network);
    return network;
}

void initiateSteppingStoneCycle (steppingStoneCycle *&domain, transportationProblem network, int indexOfTarget_i, int indexOfTarget_j) {
    supplierDestination *pSupplier = network.firstSupplier;
    supplierDestination *pDestination = network.firstDestination;
    int nSupplier = 0, nDestination = 0;

    //counting suppliers and destinations
    while (!(pSupplier == NULL)) {
        nSupplier ++;
        pSupplier = pSupplier->next;
    }
    while (!(pDestination == NULL)) {
        nDestination ++;
        pDestination = pDestination->next;
    }

    shipment *shipmentData[nSupplier][nDestination], *pShipment;
    for (int i = 0; i < nSupplier; i++) {
        for (int j = 0; j < nDestination; j++) {
            pShipment = network.firstShipment;
            while (pShipment->i < i) { pShipment = pShipment->down; }
            while (pShipment->j < j) { pShipment = pShipment->right; }
            shipmentData[i][j] = new shipment;
            shipmentData[i][j]->valid = true;
            shipmentData[i][j]->i = pShipment->i;
            shipmentData[i][j]->j = pShipment->j;
            shipmentData[i][j]->c = pShipment->c;
            shipmentData[i][j]->x = pShipment->x;
        }
    }

    //connecting all shipments that had been created
    for (int i = 0; i < nSupplier; i++) {
        for (int j = 0; j < nDestination; j++) {
            shipmentData[i][j]->left = (j-1) >= 0 ? shipmentData[i][j-1] : NULL;
            shipmentData[i][j]->right = (j+1) < nDestination ? shipmentData[i][j+1] : NULL;
            shipmentData[i][j]->up = (i-1) >= 0 ? shipmentData[i-1][j] : NULL;
            shipmentData[i][j]->down = (i+1) < nSupplier ? shipmentData[i+1][j] : NULL;
        }
    }

    domain = new steppingStoneCycle;
    domain->target = shipmentData[indexOfTarget_i][indexOfTarget_j];
    domain->current = domain->target; //initial condition
}

void rearrangeSteppingStoneCycle (steppingStoneCycle *&result) {
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

steppingStoneCycle *findSteppingStoneCycle (steppingStoneCycle *&domain, string direction = "") {
    steppingStoneCycle *result;

    string directions[] = {"left", "up", "right", "down"};
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

    shipment *pShipmentCycle = domain->cycle;
    if (!(pShipmentCycle == NULL)) {
        while (pShipmentCycle->next != NULL) {
            pShipmentCycle = pShipmentCycle->next;
        }
    }

    shipment *pShipment, *pShipmentCheckpoint;
    pShipmentCheckpoint = domain->current;
    for (string direction_ : directions) {
        if (direction_  != complementDirection) {
            pShipment = direction_ == "up" ? domain->current->up 
                : direction_ == "right" ? domain->current->right
                : direction_ == "down" ? domain->current->down
                : domain->current->left;

            while ((!(pShipment == NULL)) && pShipment->x == 0) {
                if (pShipment->i == domain->target->i && pShipment->j == domain->target->j) {
                    pShipmentCycle->next = pShipment;
                    domain->current = pShipment;
                    domain->solved = true;
                    rearrangeSteppingStoneCycle(domain);
                    return domain;
                }
                pShipment = direction_ == "up" ? pShipment->up 
                    : direction_ == "right" ? pShipment->right
                    : direction_ == "down" ? pShipment->down
                    : pShipment->left;
            }

            if (!(pShipment == NULL)) {
                domain->current = pShipment;
                if (pShipmentCycle == NULL) {
                    domain->cycle = pShipment;
                } else {
                    pShipmentCycle->next = pShipment;
                }
                
                result = findSteppingStoneCycle(domain, direction_);
                if (result->solved) {
                    return result;
                } else { // revert
                    domain->current = pShipmentCheckpoint;
                    if (pShipmentCycle == NULL) {
                        domain->cycle = NULL;
                    } else {
                        pShipmentCycle->next = NULL;
                    }
                }
            }
        }
    }
    return domain;
}

transportationProblem steppingStone (transportationProblem network) {
    steppingStoneCycle *domain, *result;
    int indexOfCellTarget_i, indexOfCellTarget_j, minScore, score, countCellChain, countIteration = 0;
    shipment *pShipment, *pShipmentCycle;

    supplierDestination *pFactory, *pStorage;
    int nFactories, nStorages;

    int amountChanges;

    do {
        // counting factories and storages
        pShipment = network.firstShipment;
        pFactory = network.firstSupplier;
        pStorage = network.firstDestination;
        nFactories = 0;
        nStorages = 0;
        minScore = 0;
        while (!(pFactory == NULL)) {
            nFactories ++;
            pFactory = pFactory->next;
        }
        while (!(pStorage == NULL)) {
            nStorages ++;
            pStorage = pStorage->next;
        }

        for (int i = 0; i < nFactories; i++) {
            for (int j = 0; j < nStorages; j++) {
                while (pShipment->i < i) pShipment = pShipment->down;
                while (pShipment->j < j) pShipment = pShipment->right;

                initiateSteppingStoneCycle(domain, network, pShipment->i, pShipment->j);
                if (pShipment->x == 0) {
                    result = findSteppingStoneCycle(domain);
                    if (result->solved) {
                        pShipmentCycle = result->cycle;
                        countCellChain = 0;
                        score = 0;
                        while (!(pShipmentCycle == NULL)) {
                            countCellChain ++;
                            score += countCellChain % 2 == 0 ? pShipmentCycle->c : ((-1)*pShipmentCycle->c);
                            pShipmentCycle = pShipmentCycle->next;
                        } 
                        if (score < minScore) {
                            minScore = score;
                            indexOfCellTarget_i = i;
                            indexOfCellTarget_j = j;
                        }
                    }
                }
                pShipment = network.firstShipment;
            }
        }

        if (minScore < 0) {
            initiateSteppingStoneCycle(domain, network, indexOfCellTarget_i, indexOfCellTarget_j);
            result = findSteppingStoneCycle(domain);

            pShipmentCycle = result->cycle;
            amountChanges = NULL; // nilai minimum dari variable keluar
            countCellChain = 0;

            while (!(pShipmentCycle == NULL)) {
                if (++countCellChain % 2 != 0) {
                    if ((!amountChanges) || pShipmentCycle->x < amountChanges) {
                        amountChanges = pShipmentCycle->x;
                    }
                }
                pShipmentCycle = pShipmentCycle->next;
            }

            pShipmentCycle = result->cycle;
            countCellChain = 0;
            while(!(pShipmentCycle == NULL)) {
                countCellChain++;
                pShipmentCycle->x += countCellChain % 2 == 0 ? amountChanges : ((-1)*(amountChanges));
                pShipmentCycle = pShipmentCycle->next;
            }

            //update network
            pShipmentCycle = result->cycle;
            while (!(pShipmentCycle == NULL)) {
                
                pShipment = network.firstShipment;
                while (pShipment->i < pShipmentCycle->i) {
                    pShipment = pShipment->down;
                }
                while (pShipment->j < pShipmentCycle->j) {
                    pShipment = pShipment->right;
                }

                pShipment->x = pShipmentCycle->x;
                pShipmentCycle = pShipmentCycle->next;
            }
            network.z = zValue(network);
            cout << endl << endl;
            display_system(network, "Stepping Stone", ++countIteration);
        }
    
    } while (minScore < 0);

    
    cout << endl << endl;
    cout << "finished!" << endl;

    return network;
}

int main() {
    transportationProblem network;
    shipment shipmentData[100][100];
    supplierDestination supplier[100], destination[100];
    int numberOfSupplier = 0, numberOfDestination = 0, totalDemand = 0, totalSupply = 0;
    
    do {

        cout << "Number of Suppliers: "; cin >> numberOfSupplier;
        cout << "Number of Destinations: " ; cin >> numberOfDestination;

        //input a of supply for each supplier
        for (int i = 0; i < numberOfSupplier; i++) {
            cout << "Supplier - " << (i+1) << ": " ; cin >> supplier[i].a;
            supplier[i].aTemp  = supplier[i].a;
            supplier[i].valid = true;
            supplier[i].index = i;
            totalSupply += supplier[i].a;
            if ((i-1 >= 0) && supplier[i-1].valid) {
                supplier[i-1].next = &supplier[i];
            }
            if (i == 0) {
                network.firstSupplier = &supplier[i];
            }
        }
        //input a of demand for each destination
        for (int i = 0; i < numberOfDestination; i++) {
            cout << "Destination - " << (i+1) << ": " ; cin >> destination[i].a;
            destination[i].aTemp  = destination[i].a;
            destination[i].valid = true;
            destination[i].index = i;
            totalDemand += destination[i].a;
            if ((i-1 >= 0) && destination[i-1].valid) {
                destination[i-1].next = &destination[i];
            }
            if (i == 0) {
                network.firstDestination = &destination[i];
            }
        }
        //totalDemand and totalSupply must be equal (balance)
        if (totalDemand != totalSupply) {
            cout << "total demand and total supply must be equal or balance" << endl << endl;
        } else {
            //input c for each shipment
            for (int i = 0; i < numberOfSupplier; i++) {
                for (int j = 0; j < numberOfDestination; j++) {
                    shipmentData[i][j].i = i;
                    shipmentData[i][j].j = j;
                    shipmentData[i][j].valid = true;
                    cout << "Supplier " << (i+1) << " - Destination " << (j+1) << ": "; cin >> shipmentData[i][j].c;
                    if (i==0 && j==0) {
                        network.firstShipment = &shipmentData[i][j];
                    }
                }
            }
            //Connecting all cells
            for (int i = 0; i < numberOfSupplier; i++) {
                for (int j = 0; j < numberOfDestination; j++) {
                    // up
                    if ((i-1 >= 0) && shipmentData[i-1][j].valid) {
                        shipmentData[i][j].up = &shipmentData[i-1][j];
                    }
                    // down
                    if ((i+1 < numberOfSupplier) && shipmentData[i+1][j].valid) {
                        shipmentData[i][j].down = &shipmentData[i+1][j];
                    }
                    //left
                    if ((j-1 >= 0) && shipmentData[i][j-1].valid) {
                        shipmentData[i][j].left = &shipmentData[i][j-1];
                    }
                    //right
                    if ((j+1 < numberOfDestination) && shipmentData[i][j+1].valid) {
                        shipmentData[i][j].right = &shipmentData[i][j+1];
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
        for (int i = 0; i < numberOfSupplier; i++) {
            for (int j = 0; j < numberOfDestination; j++) {
                shipmentData[i][j].valid = false;
                shipmentData[i][j].i = 0;
                shipmentData[i][j].j = 0;
                shipmentData[i][j].c = 0;
                shipmentData[i][j].x = 0;
                shipmentData[i][j].up = NULL;
                shipmentData[i][j].down = NULL;
                shipmentData[i][j].left = NULL;
                shipmentData[i][j].right = NULL;
                shipmentData[i][j].next = NULL;
            }
        }
        for (int i = 0; i < numberOfSupplier; i++) {
            supplier[i].valid = false;
            supplier[i].a = NULL;
            supplier[i].aTemp  = NULL;
            supplier[i].index = NULL;
            supplier[i].next = NULL;
        }
        numberOfSupplier = 0;
        totalSupply = 0;
        for (int i = 0; i < numberOfDestination; i++) {
            destination[i].valid = false;
            destination[i].a = NULL;
            destination[i].aTemp  = NULL;
            destination[i].index = NULL;
            destination[i].next = NULL;
        }
        totalDemand = 0;
        numberOfDestination = 0;
        network.firstShipment = NULL;
        network.firstSupplier = NULL;
        network.firstDestination = NULL;
        network.z = NULL;

        system("CLS");
    } while (true);

    system("pause");
    return 0;
}
