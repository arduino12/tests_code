function rgb(r,g,b)
	pwm.setduty(8,r)
	pwm.setduty(6,g)
	pwm.setduty(7,b)
end
pwm.setup(8,500,255) pwm.start(8)
pwm.setup(6,500,255) pwm.start(6)
pwm.setup(7,500,255) pwm.start(7)
rgb(0,0,0)

t= cjson.decode("{\"key\":\"value\"}")
for k,v in pairs(t) do print(k,v) end

function leds(m)
	gpio.write(4, gpio.HIGH)
	gpio.write(3, gpio.HIGH)
	gpio.write(2, gpio.HIGH)
	gpio.write(4, gpio.HIGH)
	gpio.write(5, gpio.HIGH)
	gpio.write(0, gpio.HIGH)
	pwm.setduty(6,g)
	pwm.setduty(7,b)
end

s:close() --CLOSE!
ws2801.init()
s=net.createServer(net.TCP)
s:listen(8040,function(c)
	c:on("receive",function(c,d)
		j = cjson.decode(d)
		print(j)
		ws2801.write(string.char(j["r"],j["g"],j["b"],0,0,0,0,0,0,0,0,0,0,0,0,0,0,0))
		c:close()
	end)
end)
s:close() --CLOSE!

function reconnect()
    print "Connecting..."
    c=net.createConnection(net.TCP,0)
    c:connect(8888,"192.168.1.100")
    c:on("connection",function(c)
        tmr.stop(0)
        print "Connected..!"
    end)
    c:on("disconnection",function(c)
        c:close()
        c=nil
        tmr.alarm(0,5000,0,reconnect)
    end)
    c:on("receive",function(c,p)
        print("Received: "..p)
    end)
end
reconnect()

c=net.createConnection(net.UDP,0)
function reconnect()
    tmr.alarm(0,1000,1,function()
        print "t"
        c:close()
        c:connect(8888,"192.168.1.100")
        print "p"
    end)
end
c:on("connection",function(c)
    print "connection"
    tmr.stop(0)
end)
c:on("reconnection",function(c)
    print "reconnection"
end)
c:on("disconnection",function(c)
    print "disconnection"
    reconnect()
end)
c:on("sent",function(c)
    print "sent"
end)
c:on("receive",function(c,p)
	print ("receive: "..p)
end)

    node.output(function(p)
        if(c~=nil) then
            c:send(p)
        end
    end,
    1)

_dir=function()
	local k,v,l
	print("~~~File ".."list START~~~")
	for k,v in pairs(file.list()) do
		l = string.format("%-15s",k)
		print(l.." : "..v.." bytes")
	end
	print("~~~File ".."list END~~~")
end
_dir()
_dir=nil


function ff(p)
	c:send(p)
	node.output(nil)
	node.output(ff,1)
end
node.output(ff,1)

dd = ""
node.output(function(p)
	dd=dd..p
	if string.sub(p,-2) == "> " then
		c:send(p)
	end
end,
0)


e=\"\" node.output(function(d) e=e..d if string.sub(d,-2)==\"> \" then c:send(e) e=\"\" end end,1)

dd = "" node.output(function(p) dd = dd..p end,1)
function ff(p) c:send(p) node.output(nil) node.output(ff,1) end node.output(ff,1)



bb='ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'
function from64(d)
	d=string.gsub(d,"[^"..bb.."=]","")
	return (d:gsub(".",function(x)
			if(x=="=") then
				return ""
			end
			local r,f="",(bb:find(x)-1)
			for i=6,1,-1 do
				r=r..(f%2^i-f%2^(i-1)>0 and "1" or "0")
			end
			return r;
		end):gsub("%d%d%d?%d?%d?%d?%d?%d?", function(x)
			if(#x~=8) then
				return ""
			end
			local c=0
			for i=1,8 do
				c=c+(x:sub(i,i)=="1" and 2^(8-i) or 0)
			end
			return string.char(c)
		end))
end

=from64(to64("123123123123123123"))


units.unit_120.soldiers.arad_all_rgb_leds._helper.set_channels({1:255,3:255,4:255,6:255,12:255,13:50})

def h(s): print "".join(chr(int(i,16)) for i in s.split())




r,u,t=file.fsinfo() print("Total : "..t.." bytes\r\nUsed  : "..u.." bytes\r\nRemain: "..r.." bytes\r\n") r=nil u=nil t=nil



20 5F 64 69 72 3D 66 75 6E 63 74 69 6F 6E 28 29 20 6C 6F 63 61 6C 20 6B 2C 76 2C 6C 20 70 72 69 6E 74 28 22 7E 7E 7E 46 69 6C 65 20 22 2E 2E 22 6C 69 73 74 20 53 54 41 52 54 7E 7E 7E 22 29 20 66 6F 72 20 6B 2C 76 20 69 6E 20 70 61 69 72 73 28 66 69 6C 65 2E 6C 69 73 74 28 29 29 20 64 6F 20 6C 20 3D 20 73 74 72 69 6E 67 2E 66 6F 72 6D 61 74 28 22 25 2D 31 35 73 22 2C 6B 29 20 70 72 69 6E 74 28 6C 2E 2E 22 20 3A 20 22 2E 2E 76 2E 2E 22 20 62 79 74 65 73 22 29 20 65 6E 64 20 70 72 69 6E 74 28 22 7E 7E 7E 46 69 6C 65 20 22 2E 2E 22 6C 69 73 74 20 45 4E 44 7E 7E 7E 22 29 20 65 6E 64 20 5F 64 69 72 28 29 20 5F 64 69 72 3D 6E 69 6C 0D
20 5F 64 69 72 3D 66 75 6E 63 74 69 6F 6E 28 29 20 6C 6F 63 61 6C 20 6B 2C 76 2C 6C 20 70 72 69 6E 74 28 22 7E 7E 7E 46 69 6C 65 20 22 2E 2E 22 6C 69 73 74 20 53 54 41 52 54 7E 7E 7E 22 29 20 66 6F 72 20 6B 2C 76 20 69 6E 20 70 61 69 72 73 28 66 69 6C 65 2E 6C 69 73 74 28 29 29 20 64 6F 20 6C 20 3D 20 73 74 72 69 6E 67 2E 66 6F 72 6D 61 74 28 22 25 2D 31 35 73 22 2C 6B 29 20 70 72 69 6E 74 28 6C 2E 2E 22 20 3A 20 22 2E 2E 76 2E 2E 22 20 62 79 74 65 73 22 29 20 65 6E 64 20 70 72 69 6E 74 28 22 7E 7E 7E 46 69 6C 65 20 22 2E 2E 22 6C 69 73 74 20 45 4E 44 7E 7E 7E 22 29 20 65 6E 64 20 5F 64 69 72 28 29 20 5F 64 69 72 3D 6E 69 6C 0D 0A 7E 7E 7E 46 69 6C 65 20 6C 69 73 74 20 53 54 41 52 54 7E 7E 7E 0D 0A 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 3A 20 33 33 37 35 31 30 34 30 20 62 79 74 65 73 0D 0A 63 6C 69 65 6E 74 2E 6C 75 61 20 20 20 20 20 20 3A 20 35 31 35 20 62 79 74 65 73 0D 0A 7E 7E 7E 46 69 6C 65 20 6C 69 73 74 20 45 4E 44 7E 7E 7E 0D 0A 3E 20 
20 5F 64 69 72 3D 66 75 6E 63 74 69 6F 6E 28 29 20 6C 6F 63 61 6C 20 6B 2C 76 2C 6C 20 70 72 69 6E 74 28 22 7E 7E 7E 46 69 6C 65 20 22 2E 2E 22 6C 69 73 74 20 53 54 41 52 54 7E 7E 7E 22 29 20 66 6F 72 20 6B 2C 76 20 69 6E 20 70 61 69 72 73 28 66 69 6C 65 2E 6C 69 73 74 28 29 29 20 64 6F 20 6C 20 3D 20 73 74 72 69 6E 67 2E 66 6F 72 6D 61 74 28 22 25 2D 31 35 73 22 2C 6B 29 20 70 72 69 6E 74 28 6C 2E 2E 22 20 3A 20 22 2E 2E 76 2E 2E 22 20 62 79 74 65 73 22 29 20 65 6E 64 20 70 72 69 6E 74 28 22 7E 7E 7E 46 69 6C 65 20 22 2E 2E 22 6C 69 73 74 20 45 4E 44 7E 7E 7E 22 29 20 65 6E 64 20 5F 64 69 72 28 29 20 5F 64 69 72 3D 6E 69 6C 0D 0A 7E 7E 7E 46 69 6C 65 20 6C 69 73 74 20 53 54 41 52 54 7E 7E 7E 0D 0A 3E 20 


_dir=function() local k,v,l print("~~~File ".."list START~~~") for k,v in pairs(file.list()) do l = string.format("%-15s",k) print(l.." : "..v.." bytes") end print("~~~File ".."list END~~~") end _dir() _dir=nil
