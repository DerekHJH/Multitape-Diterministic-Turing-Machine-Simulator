; This example program checks if the input string belongs to the language of case 1.
; Input: a string of a's and b's, e.g. 'abbabb'

; the finite set of states
#Q = {q0,q1,q2,q3,q4,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject}
; q1, q2 should copy the first part of a's and b's onto the second tape.
; q3 moves the second tape head back to the front
; q4 start to compare the strings on tape 0 and tape1

; the finite set of input symbols
#S = {a,b}

; the complete set of tape symbols
#G = {a,b,_,t,r,u,e,f,a,l,s}

; the start state
#q0 = q0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2

; the transition functions

; State q0: start state. We do nothing here.
q0 a_ a_ ** q1
q0 b_ b_ ** reject
q0 __ __ ** reject

; State q1: clean the first a's on tape 0 and copy it on the corresponding position on tape 1.
q1 a_ _a rr q1
q1 __ __ ** reject
q1 b_ b_ ** q2

; State q2: clean the first b's on tape 0 and copy it on the corresponding position on tape 1.
q2 b_ _b rr q2
q2 __ __ ** reject
q2 a_ a_ *l q3

; State q3: move the head 1 back to the front
q3 aa aa *l q3
q3 ab ab *l q3
q3 a_ a_ *r q4

; State q4: compare strings on tape 0 and tape 1
q4 aa __ rr q4
q4 bb __ rr q4
q4 __ __ ** accept
q4 ab ab ** reject
q4 ba ba ** reject
q4 a_ a_ ** reject
q4 _a _a ** reject
q4 b_ b_ ** reject
q4 _b _b ** reject

; State accept*: write 'true' on tape 0
accept __ t_ r* accept2
accept2 __ r_ r* accept3
accept3 __ u_ r* accept4
accept4 __ e_ ** halt_accept

; State reject*: write 'false' on tape 0
reject ab __ rr reject
reject ba __ rr reject
reject a_ __ r* reject
reject b_ __ r* reject
reject _a __ *r reject
reject _b __ *r reject
reject __ f_ r* reject2
reject2 __ a_ r* reject3
reject3 __ l_ r* reject4
reject4 __ s_ r* reject5
reject5 __ e_ ** halt_reject
