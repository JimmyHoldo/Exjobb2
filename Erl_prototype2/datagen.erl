-module(datagen).

-export([start/1]).

start([Time]) ->
  io:format("supervisor started datagen!~n", []),
  Pid = spawn_link(fun() -> loop(0, Time) end),
  {ok, Pid}.

loop(N, Time) ->
  writerloop ! {msgToWriter, ("1 Msg:" ++ integer_to_list(N+100))},
  timer:sleep(Time),
  loop(((N+1) rem 899), Time).
