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
q3 =1_ _1_ r*l q7

; State q4: copying.
q4 11_ 111 *lr q4
q4 1__ 1__ *r* q5

; State q5: move head 1 back to the back for the next copying.
q5 11_ 11_ *r* q5
q5 1__ 1__ *l* q6

; State q6: clean one 1 from tape 0.
q6 11_ _1_ r** q3

; State q7: compare '1's on tape 0 and tape 2.
q7 111 _1_ r*l q7
q7 _1_ _1_ *** accept
q7 _11 _11 *** reject
q7 =11 =11 *** reject
q7 x11 x11 *** reject
q7 11_ 11_ *** reject
q7 =1_ =1_ *** reject
q7 x1_ x1_ *** reject

; State accept*: write 'true' on 1st tape
accept _1_ t1_ r** accept2
accept2 _1_ r1_ r** accept3
accept3 _1_ u1_ r** accept4
accept4 _1_ e1_ *** halt_accept

; State reject*: write 'false' on 1st tape
reject _11 _1_ r*l reject
reject =11 _1_ r*l reject
reject x11 _1_ r*l reject
reject 111 _1_ r*l reject
reject =1_ _1_ r*l reject
reject x1_ _1_ r*l reject
reject 11_ _1_ r*l reject
reject _1_ f1_ r** reject2
reject2 _1_ a1_ r** reject3
reject3 _1_ l1_ r** reject4
reject4 _1_ s1_ r** reject5
reject5 _1_ e1_ *** halt_reject
