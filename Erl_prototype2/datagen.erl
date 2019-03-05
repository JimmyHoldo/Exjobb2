-module(datagen).

-export([start/1]).

start(Time) ->
  io:format("supervisor started datagen!~n", []),
  Pid = spawn_link(fun() -> loop(1, Time) end),
  {ok, Pid}.

loop(N, Time) ->
  writerloop ! {msgToWriter, ("1: " ++ integer_to_list(N))},
  timer:sleep(Time),
  loop(((N+1) rem 1000), Time).
