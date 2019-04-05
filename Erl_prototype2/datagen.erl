-module(datagen).

-export([start/1]).

start([Time]) ->
  %erlang:garbage_collect(self()),
  %io:format("supervisor started datagen! ~p~n", [erlang:process_info(self(),memory)]),
  Pid = spawn_link(fun() -> loop(0, Time) end),
  {ok, Pid}.

loop(N, Time) ->
  writerloop ! {msgToWriter, ("1 Msg:" ++ integer_to_list(N+100))},
  timer:sleep(Time),
  loop(((N+1) rem 899), Time).
