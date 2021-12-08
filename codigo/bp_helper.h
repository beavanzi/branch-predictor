#ifndef _BP_HELPER_H_
#define _BP_HELPER_H_
#include <bitset>
#include <vector>
#include <math.h>
#define LEN_BHR 12


typedef struct stateMachine {
    bitset<2> state;
};

typedef vector<stateMachine> simplePHT;

typedef vector<simplePHT> composedPHTs;

typedef struct simpleBHR {
    unsigned int actualHistoric:LEN_BHR;
};

typedef vector<simpleBHR> BHT;


// Auxiliaries functions

void initializePHT(simplePHT &PHT, int len_pht) {
    for (int i=0; i<len_pht; i++) {
        //inicializando cada maquina de estado de PHT
        stateMachine EM;
        EM.state = 0b00;
        PHT.push_back(EM);
    }
}

bool isTaken(stateMachine machine){
    return machine.state.to_ulong() >= 2;
}

int getMask(int n_bits) {
    return pow(2, (float)n_bits) - 1;
}

uintptr_t getBitsLessSignificant(int n_bits, uintptr_t inst) {
    return inst & getMask(n_bits);
}

int offsetToLeft(int big_len, int small_len) {
    if (big_len > small_len) {
        return big_len - small_len;    
    }

    return 0;
}

int getLenByExponent(uintptr_t n) {
    return (int) log2(n) + 1;
}

uintptr_t getBitsMoreSignificant(int n_bits, uintptr_t inst) {
    int mask = getMask(n_bits);
    int inst_len = getLenByExponent(inst);
    int offset = offsetToLeft(inst_len, n_bits);

    inst = inst >> offset;
    return inst & mask;
}

void updateToTaken(simplePHT &PHT, simpleBHR &BHR) {
    if (PHT[BHR.actualHistoric].state.to_ulong() < 3){
        PHT[BHR.actualHistoric].state = PHT[BHR.actualHistoric].state.to_ulong() + 1;
    }

    BHR.actualHistoric = BHR.actualHistoric << 1;
    BHR.actualHistoric = BHR.actualHistoric + 1; 
}

void updateToNotTaken(simplePHT &PHT, simpleBHR &BHR) {
    if (PHT[BHR.actualHistoric].state.to_ulong() > 0){
        PHT[BHR.actualHistoric].state = PHT[BHR.actualHistoric].state.to_ulong() - 1;
    }

    BHR.actualHistoric = BHR.actualHistoric << 1;
}

#endif /* _BP_HELPER_H_ */
