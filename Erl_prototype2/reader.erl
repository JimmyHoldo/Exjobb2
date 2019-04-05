-module(reader).

-export([start/2]).

start(Arg, ExtPrg) ->
    %erlang:garbage_collect(self()),
    %io:format("supervisor started reader! ~p~n", [erlang:process_info(self(),memory)]),
    Pid = spawn_link(fun() -> init(Arg, ExtPrg) end),
    {ok, Pid}.

init(Arg, ExtPrg) ->
	PortReader = open_port({spawn_executable, ExtPrg}, [{packet, 2}]),
	Serial_fd = serialport:open_port_serial(PortReader, Arg),
    case Serial_fd of
        255 -> exit(normal);
        _   -> serialport:initport(PortReader, Serial_fd),
    	       loop(PortReader, Serial_fd)
	end.

loop(PortReader, Serial_fd) ->
    Msg = serialport:read_from_zigbee(PortReader, Serial_fd),
    writerloop ! {msgToWriter, Msg},
    loop(PortReader, Serial_fd).
