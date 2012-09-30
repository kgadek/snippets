-module(partial_tests).

-compile(export_all).
-include_lib("eunit/include/eunit.hrl").
-import(partial,[
		curry/2,curry/3,
		uncurry/2,uncurry/3,
		papply/3,papply/4,
		papply_C/2]).

fun0() ->
	42.
fun1(X) ->
	X+3.
fun2(X,Y) ->
	X+Y*Y.
fun3(X,Y,Z) ->
	X+Y*Y-Z.


curry_test() ->
	42 = curry(?MODULE, fun0, 0),
	42 = curry(fun() -> fun0() end, 0),

	F1a = curry(?MODULE, fun1, 1),
	F1b = curry(fun(X) -> fun1(X) end, 1),
	5 = F1a(2),
	5 = F1b(2),

	F2a = curry(?MODULE, fun2, 2),
	F2b = curry(fun(X,Y) -> fun2(X,Y) end, 2),
	7 = (F2a(3))(2),
	7 = (F2b(3))(2),

	F3a = curry(?MODULE, fun3, 3),
	F3b = curry(fun(X,Y,Z) -> fun3(X,Y,Z) end, 3),
	2 = ((F3a(3))(2))(5),
	2 = ((F3b(3))(2))(5).

uncurry_test() ->
	F0a = uncurry(?MODULE, fun0, 0),
	F0b = uncurry(fun() -> fun0() end, 0),
	42 = F0a([]),
	42 = F0b([]),

	F1a = uncurry(?MODULE, fun1, 1),
	F1b = uncurry(fun(X) -> fun1(X) end, 1),
	5 = F1a([2]),
	5 = F1b([2]),

	F2a = uncurry(?MODULE, fun2, 2),
	F2b = uncurry(fun(X) -> fun(Y) -> fun2(X,Y) end end, 2),
	7 = F2a([3,2]),
	7 = F2b([3,2]),

	F3a = uncurry(?MODULE, fun3, 3),
	F3b = uncurry(fun(X) -> fun(Y) -> fun(Z) -> fun3(X,Y,Z) end end end, 3),
	2 = F3a([3,2,5]),
	2 = F3b([3,2,5]).

papply_test() ->
	42 = papply(fun() -> fun0() end, 0, []),
	42 = papply(?MODULE, fun0, 0, []),

	5 = papply(?MODULE, fun1, 1, [2]),
	5 = papply(fun(X) -> fun1(X) end, 1, [2]),
	F1a0 = papply(?MODULE, fun1, 1, []),
	F1a1 = papply(fun(X) -> F1a0(X) end, 1, []),
	5 = papply(F1a0, 1, [2]),
	5 = papply(F1a1, 1, [2]),
	F1b0 = papply(fun(X) -> fun1(X) end, 1, []),
	F1b1 = papply(F1b0, 1, []),
	5 = papply(F1b0, 1, [2]),
	5 = papply(F1b1, 1, [2]),

	7 = papply(?MODULE, fun2, 2, [3,2]),
	7 = papply(fun(X,Y) -> fun2(X,Y) end, 2, [3,2]),
	F2a0 = papply(?MODULE, fun2, 2, [3]),
	7 = F2a0(2),
	7 = papply(fun(Y) -> F2a0(Y) end, 1, [2]),

	2 = papply(?MODULE, fun3, 3, [3,2,5]),
	F3a0 = papply(?MODULE, fun3, 3, [3,2]),
	F3a1 = papply(?MODULE, fun3, 3, [3]),
	F3a2 = papply(?MODULE, fun3, 3, []),
	2 = papply_C(F3a0, [5]),
	2 = papply_C(F3a1, [2,5]),
	2 = papply_C(F3a2, [3,2,5]).
