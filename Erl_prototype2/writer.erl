-module(writer).

-export([start/2]).

start(Arg, ExtPrg) ->
  erlang:garbage_collect(self()),
  io:format("supervisor started writer! Arg=~s ~p~n",
            [Arg, erlang:process_info(self(),memory)]),
  Pid = spawn_link(fun() -> init(Arg, ExtPrg) end),
  register(writerloop, Pid),
  {ok, Pid}.

init(Arg, ExtPrg) ->
	PortWriter = open_port({spawn_executable, ExtPrg}, [{packet, 2}]),
	Serial_fd = serialport:open_port_serial(PortWriter, Arg),
    case Serial_fd of
        255 -> exit(normal);
        _   -> serialport:initport(PortWriter, Serial_fd),
    	       loop(PortWriter, Serial_fd)
	end.

loop(PortWriter, Serial_fd) ->
  receive
		{msgToWriter, Msg} ->
            io:format("~s~n", [Msg]),
			serialport:write_to_zigbee(PortWriter, [Serial_fd, Msg, 0])
  end,
  loop(PortWriter, Serial_fd).

% c(writer). c(serialport). writer:start("/dev/ttyUSB2", "readerwriterprg").
