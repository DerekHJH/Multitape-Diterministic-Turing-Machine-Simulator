; This example program checks if the input string belongs to the language of case 2.
; Input: a string of 1's and x, =, e.g. '11x111=111111'

; the finite set of states
#Q = {q0,q1,q2,q3,q4,q5,q6,q7,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject}

; the finite set of input symbols
#S = {1,x,=}

; the complete set of tape symbols
#G = {1,x,=,_,t,r,u,e,f,a,l,s}

; the start state
#q0 = q0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 3

; the transition functions

; State q0: start state. We do nothing here
q0 1__ 1__ *** q1
q0 x__ x__ *** reject
q0 =__ =__ *** reject
q0 ___ ___ *** reject

; State q1: copy the '1's to the left of 'x' to tape 1.
q1 1__ _1_ rr* q1
q1 x__ ___ r** q2
q1 =__ =__ *** reject
q1 ___ ___ *** reject

; State q2: prepare to enter q3 --- the loop of copying the '1's to the right of 'x' several times.
q2 1__ 1__ *l* q3
q2 x__ x__ *** reject
q2 =__ =__ *** reject
q2 ___ ___ *** reject

; State q3: start the loop of copying the '1's to the right of 'x' several times.
q3 11_ 11_ *** q4
q3 =1_ ___ r*l q7
q3 x1_ x__ *** reject
q3 _1_ ___ *** reject

; State q4: copying.
q4 11_ 111 *lr q4
q4 1__ 1__ *r* q5

; State q5: move head 1 back to the back for the next copying.
q5 11_ 11_ *r* q5
q5 1__ 1__ *l* q6

; State q6: clean one 1 from tape 0.
q6 11_ _1_ r** q3

; State q7: compare '1's on tape 0 and tape 2.
q7 1_1 ___ r*l q7
q7 ___ ___ *** accept
q7 __1 ___ *** reject
q7 =_1 =__ *** reject
q7 x_1 x__ *** reject
q7 1__ 1__ *** reject
q7 =__ =__ *** reject
q7 x__ x__ *** reject

; State accept*: write 'true' on 1st tape
accept ___ t__ r** accept2
accept2 ___ r__ r** accept3
accept3 ___ u__ r** accept4
accept4 ___ e__ *** halt_accept

; State reject*: write 'false' on 1st tape
reject =__ ___ r** reject
reject x__ ___ r** reject
reject 1__ ___ r** reject
reject ___ f__ r** reject2
reject2 ___ a__ r** reject3
reject3 ___ l__ r** reject4
reject4 ___ s__ r** reject5
reject5 ___ e__ *** halt_reject
