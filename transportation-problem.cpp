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
            shipmentData[i][j]->c = pointerShipment->c;
            shipmentData[i][j]->x = pointerShipment->x;
            pointerShipment = network.firstShipment;
        }
    }
    for (int i = 0; i < nSupplier; i++) {
        supplier[i] = new supplierDestination;
        while (pointerSupplier->index < i) {
            pointerSupplier = pointerSupplier->next;
        }
        supplier[i]->valid = true;
        supplier[i]->a = pointerSupplier->a;
        supplier[i]->aTemp  = pointerSupplier->aTemp; 
        supplier[i]->index = pointerSupplier->index;
        pointerSupplier = network.firstSupplier;
    }
    for (int i = 0; i < nDestination; i++) {
        destination[i] = new supplierDestination;
        while (pointerDestination->index < i) {
            pointerDestination = pointerDestination->next;
        }
        destination[i]->valid = true;
        destination[i]->a = pointerDestination->a;
        destination[i]->aTemp  = pointerDestination->aTemp; 
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

void rearrangeSteppingStoneShipmentCycle (steppingStoneCycle *&result) {
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

steppingStoneCycle *checkSteppingStone (steppingStoneCycle *&domain, string direction = "") {
    shipment *pShipment, *pShipmentCycle, *pCheckpointShipmentCycle;
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

    pShipmentCycle = domain->cycle;
    if (!(pShipmentCycle == NULL)) {
        while (pShipmentCycle->next != NULL) {
            pShipmentCycle = pShipmentCycle->next;
        }
    }
    pCheckpointShipmentCycle = pShipmentCycle;

    for (string direction_ : directions) {
        if (direction_ != complementDirection) {
            if (direction_ == "up") {
                pShipment = current->up;
                while ((!(pShipment == NULL)) && pShipment->x == 0) {
                    if ((!(pShipment == NULL)) && (pShipment->i == target->i && pShipment->j == target->j)) {
                        if (!(pShipmentCycle == NULL)) {
                            pShipmentCycle->next = pShipment;
                        } else {
                            domain->cycle = pShipment;
                        }
                        domain->current = pShipment;
                        domain->level = level + 1;
                        return domain;
                    }
                    pShipment = pShipment->up;
                }
                if (!(pShipment == NULL) && pShipment->x > 0) {
                    domain->current = pShipment;
                    domain->level = level + 1;
                    if (!(pShipmentCycle == NULL)) {
                        pShipmentCycle->next = pShipment;
                    } else {
                        domain->cycle = pShipment;
                    }
                    
                    result = checkSteppingStone(domain, "up");
                    if (result->valid) {
                        return result;
                    } else { //revert
                        domain->current = current;
                        domain->level = level;
                        if (!(pShipmentCycle == NULL)) {
                            pShipmentCycle->next = NULL;
                        } else {
                            domain->cycle = NULL;
                        }
                    }
                }
            } else if (direction_ == "right") {
                pShipment = current->right;
                while ((!(pShipment == NULL)) && pShipment->x == 0) {
                    if ((!(pShipment == NULL)) && (pShipment->i == target->i && pShipment->j == target->j)) {
                        if (!(pShipmentCycle == NULL)) {
                            pShipmentCycle->next = pShipment;
                        } else {
                            domain->cycle = pShipment;
                        }
                        domain->current = pShipment;
                        domain->level = level + 1;
                        return domain;
                    }
                    pShipment = pShipment->right;
                }
                if (!(pShipment == NULL) && pShipment->x > 0) {
                    domain->current = pShipment;
                    domain->level = level + 1;
                    if (!(pShipmentCycle == NULL)) {
                        pShipmentCycle->next = pShipment;
                    } else {
                        domain->cycle = pShipment;
                    }
                    
                    result = checkSteppingStone(domain, "right");
                    if (result->valid) {
                        return result;
                    } else { //revert
                        domain->current = current;
                        domain->level = level;
                        if (!(pShipmentCycle == NULL)) {
                            pShipmentCycle->next = NULL;
                        } else {
                            domain->cycle = NULL;
                        }
                    }
                }
            } else if (direction_ == "down") {
                pShipment = current->down;
                while ((!(pShipment == NULL)) && pShipment->x == 0) {
                    if ((!(pShipment == NULL)) && (pShipment->i == target->i && pShipment->j == target->j)) {
                        if (!(pShipmentCycle == NULL)) {
                            pShipmentCycle->next = pShipment;
                        } else {
                            domain->cycle = pShipment;
                        }
                        domain->current = pShipment;
                        domain->level = level + 1;
                        return domain;
                    }
                    pShipment = pShipment->down;
                }
                if (!(pShipment == NULL) && pShipment->x > 0) {
                    domain->current = pShipment;
                    domain->level = level + 1;
                    if (!(pShipmentCycle == NULL)) {
                        pShipmentCycle->next = pShipment;
                    } else {
                        domain->cycle = pShipment;
                    }
                    
                    result = checkSteppingStone(domain, "down");
                    if (result->valid) {
                        return result;
                    } else { //revert
                        domain->current = current;
                        domain->level = level;
                        if (!(pShipmentCycle == NULL)) {
                            pShipmentCycle->next = NULL;
                        } else {
                            domain->cycle = NULL;
                        }
                    }
                }
            } else if (direction_ == "left") {
                pShipment = current->left;
                while ((!(pShipment == NULL)) && pShipment->x == 0) {
                    if ((!(pShipment == NULL)) && (pShipment->i == target->i && pShipment->j == target->j)) {
                        if (!(pShipmentCycle == NULL)) {
                            pShipmentCycle->next = pShipment;
                        } else {
                            domain->cycle = pShipment;
                        }
                        domain->current = pShipment;
                        domain->level = level + 1;
                        return domain;
                    }
                    pShipment = pShipment->left;
                }
                if (!(pShipment == NULL) && pShipment->x > 0) {
                    domain->current = pShipment;
                    domain->level = level + 1;
                    if (!(pShipmentCycle == NULL)) {
                        pShipmentCycle->next = pShipment;
                    } else {
                        domain->cycle = pShipment;
                    }
                    
                    result = checkSteppingStone(domain, "left");
                    if (result->valid) {
                        return result;
                    } else { //revert
                        domain->current = current;
                        domain->level = level;
                        if (!(pShipmentCycle == NULL)) {
                            pShipmentCycle->next = NULL;
                        } else {
                            domain->cycle = NULL;
                        }
                    }
                }
            }
        }
    }

    steppingStoneCycle *dummy = new steppingStoneCycle;
    return dummy;
}

transportationProblem steppingStone (transportationProblem network) {
    steppingStoneCycle *domain, *result;
    int indexOfCellTarget_i, indexOfCellTarget_j, minScore, score, countCellChain, countIteration = 1;
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
                while (pShipment->i < i) {
                    pShipment = pShipment->down;
                }
                while (pShipment->j < j) {
                    pShipment = pShipment->right;
                }

                rebuildingNetwork(domain, network, pShipment->i, pShipment->j);
                if (pShipment->x == 0) {
                    result = NULL;
                    result = checkSteppingStone(domain);
                    if (result->valid) {
                        rearrangeSteppingStoneShipmentCycle(result);
                        pShipmentCycle = result->cycle;
                        countCellChain = 0;
                        score = 0;
                        cout << endl;
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
            rebuildingNetwork(domain, network, indexOfCellTarget_i, indexOfCellTarget_j);
            result = NULL;
            result = checkSteppingStone(domain);
            rearrangeSteppingStoneShipmentCycle(result);

            pShipmentCycle = result->cycle;
            amountChanges = NULL; // nilai minimum dari variable keluar
            countCellChain = 0;
            while (!(pShipmentCycle == NULL)) {
                countCellChain++;
                if (amountChanges) {
                    if (countCellChain % 2 != 0 && pShipmentCycle->x < amountChanges) {
                        amountChanges = pShipmentCycle->x;
                    }
                } else {
                    if (countCellChain % 2 != 0) {
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
            display_system(network, "Stepping Stone", countIteration++);
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
