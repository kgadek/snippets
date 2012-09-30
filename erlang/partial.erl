-module(partial).

-export([curry/2, curry/3, uncurry/2, uncurry/3, papply/3, papply/4, papply_C/2]).

curry_aux(F,0,Xs) -> F(lists:reverse(Xs));
curry_aux(F,N,Xs) -> fun(X) -> curry_aux(F,N-1,[X|Xs]) end.

%% @doc Curry. From (A x B x C) -> D create A -> B -> C -> D.
-spec curry(atom(), atom(), byte()) -> fun((any()) -> any()).
curry(M,F,A) when A >= 0 ->
	FF = fun(Args) -> apply(M,F,Args) end,
	curry_aux(FF,A,[]).
-spec curry(fun(), byte()) -> fun((any()) -> any()).
curry(F,A) when A >= 0 ->
	FF = fun(Args) -> apply(F,Args) end,
	curry_aux(FF,A,[]).

uncurry_aux(F,[]) -> F;
uncurry_aux(F,[X|Xs]) -> uncurry_aux(F(X), Xs).

%% @doc Uncurry. Create fun/1 taking a list of arguments.
-spec uncurry(fun(), byte()) -> fun(([any()]) -> any()).
uncurry(F,0) ->
	fun([]) -> F() end;
uncurry(F,A) ->
	fun(Xs) when length(Xs) =:= A -> uncurry_aux(F,Xs) end.
-spec uncurry(atom(), atom(), byte()) -> fun(([any()]) -> any()).
uncurry(M,F,A) ->
	fun(Xs) when length(Xs) =:= A -> apply(M,F,Xs) end.

papply_aux(F,[])     -> F;
papply_aux(F,[X|Xs]) -> papply_aux(F(X), Xs).


%% @doc Apply some arguments and create curried fun.
-spec papply(fun(),byte(),[any()]) -> any().
papply(F,A,Xs) ->
	papply_aux(curry(F,A),Xs).
-spec papply(any(),any(),byte(),[any()]) -> any().
papply(M,F,A,Xs) ->
	papply_aux(curry(M,F,A),Xs).


%% @doc Apply args on partially-applied or curried fun.
papply_C(F,Xs) ->
	papply_aux(F,Xs).
