#include "bp.h"
#include "bp_helper.h"
#include <math.h>
#define K 12
#define A 8
#define I 4
/*
 * Branch predictor state
 * Declare the global state needed by your branch predictor. All storage used
 * by your branch predictor should be here and cannot exceed 4 KiB.
 */
uintptr_t last_target;

BHT BHT_s;
composedPHTs PHTs;

/*
 * Initialize branch predictor
 * This function is called once at the beginning of the program to initialize
 * the global branch predictor state.
 */
void BP::init()
{
   /* The following flag can be helpful when debugging your branch predictor.
    * A record of the branches and your predictions can be written to the file
    * branch_trace.out. The flag can be set to the following values:
    *   TRACE_LEVEL_NONE             - do not record any branches
    *   TRACE_LEVEL_WRONG_TARGETS    - record when targets are mispredicted
    *   TRACE_LEVEL_WRONG_DIRECTIONS - record when directions are mispredicted
    *   TRACE_LEVEL_ALL              - record all branches
    * Additionally, you can also write to this file by outputting to br_trace.
    */
    int len_bht = pow(2, (float) I);
    int qtt_pht = pow(2, (float) A);
    int len_pht = pow(2, (float) K);

    initializeBHT(BHT_s, len_bht);

    initializePHTs(PHTs, qtt_pht, len_pht);

    br_trace_level = TRACE_LEVEL_NONE;
    br_trace << "SAp 2-level Branch Predictor!" << endl;
}

/*
 * Predict branch
 * This function is called when a branch occurs and the argument struct
 * contains information used to predict the branch. This function returns a
 * struct that contains the prediction.
 * input:
 *   uintptr_t  br.inst_ptr      - program counter of the branch
 *   uintptr_t  br.next_inst_ptr - next program counter of the branch
 *   uintptr_t  br.target        - branch target if known (i.e. direct branch)
 *   bool       br.uncond        - true if branch is unconditional, else false
 *   bool       br.direct        - true if branch is direct, else false
 *   bool       br.call          - true if branch is a call instruction
 *   bool       br.ret           - true if branch is a return instruction
 *   string     br.dasm          - disassembly of instruction for debugging
 * output:
 *   bool       pred.taken       - predicted direction of the branch
 *                                   true = taken; false = not taken
 *   uintptr_t  pred.target      - predicted target of the branch
 *                                   this is the taken branch target regardless
 *                                   of predicted branch direction
 */
Prediction BP::predict(EntInfo br)
{
    uintptr_t target;
    
    int instSet = getBitsMoreSignificant(I, br.inst_ptr);
    unsigned int indexToSecondLevel = BHT_s[instSet].actualHistoric;
    int instAddress = getBitsLessSignificant(A, br.inst_ptr);
    bool taken = isTaken(PHTs[instAddress][indexToSecondLevel]); 

    // Predict the taken target. If the branch is direct, just use the actual
    // target, otherwise, use the last target.
    if (br.direct) {
        target = br.target;
    } else {
        target = last_target;
    }

    // return Prediction
    return Prediction(taken, target);
}

/*
 * Update branch predictor
 * This function is called when a branch resolves and the argument struct
 * contains information used to update the branch predictor.
 * input:
 *   uintptr_t  br.inst_ptr      - program counter of the branch
 *   uintptr_t  br.next_inst_ptr - next program counter of the branch
 *   uintptr_t  br.target        - actual target of the branch
 *   bool       br.taken         - actual direction of the branch
 *                                   true = taken; false = not taken
 *   bool       br.uncond        - true if branch is unconditional, else false
 *   bool       br.direct        - true if branch is direct, else false
 *   bool       br.call          - true if branch is a call instruction
 *   bool       br.ret           - true if branch is a return instruction
 *   string     br.dasm          - disassembly of instruction for debugging
 */
void BP::update(ResInfo br)
{
    int instAddress = getBitsLessSignificant(A, br.inst_ptr);
    int instSet = getBitsMoreSignificant(I, br.inst_ptr);
    
    if (br.taken) {
        updateToTaken(PHTs[instAddress], BHT_s[instSet]);
    } else {
        updateToNotTaken(PHTs[instAddress], BHT_s[instSet]);
    }
    
    
    if (!br.direct) {
        last_target = br.target;
    }
}
