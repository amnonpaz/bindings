local sockets = require('libunixsockets')

local words_list = {
    'Test string #1',
    'Another try',
    'This is the last message in this test!'
}

local function send_list(sock, l)
    for _, v in ipairs(l) do
        local e = sock:write(v)
        if not e then
            print('Failed sending ' .. v .. ' to listener')
            break
        end

        local resp = sock:read()
        print('* Sent "' .. v .. '", listener response: "' .. tostring(resp) .. '"')
    end
end

local function print_usage(exec_name)
    print('Usage: ')
    print('    ' .. exec_name .. ' <socket_name>')
end

--[[ MAIN ]]

if (#arg < 1) or (arg[1] == '--help') then
    print_usage(arg[0])
    return
end

local sock_name = arg[1]

local s = sockets.open(sock_name)
if not s then
    print('Error opening socket') 
    return
end
s.send_list = send_list

print('Opened socket "' .. sock_name .. '"')

s:send_list(words_list)
s:close()

print('Socket closed')
