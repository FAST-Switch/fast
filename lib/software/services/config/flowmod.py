import httplib
import json

class StaticFlowPusher(object):
	def __init__(self, server):
		self.server = server

 
	def get(self, data):
		ret = self.rest_call({}, 'GET')
		return json.loads(ret[2])

 	def set(self, data):
		ret = self.rest_call(data, 'POST')
		return ret[0] == 200
	
	def remove(self, objtype, data):
		ret = self.rest_call(data, 'DELETE')
		return ret[0] == 200

	def rest_call(self, data, action):
		path = '/wm/staticflowpusher/json'
		headers = {
			'Content-type': 'application/json', 
			'Accept': 'application/json',
		}
		body = json.dumps(data)
		conn = httplib.HTTPConnection(self.server, 8080)
		conn.request(action, path, body, headers)
		response = conn.getresponse()
		ret = (response.status, response.reason, response.read())
		print ret
		conn.close()
		return ret


pusher = StaticFlowPusher('127.0.0.1')

flow0 = {
        'switch':"00:00:00:22:33:44:55:66",
	"name":"flow-mod-0",
	"cookie":"1",
	"priority":"32768",
	"active":"true",
	"ingress-port":"0",
        "eth_type":"0x86DD",
        "ipv6_src":"fe80::120b:a9ff:fef4:e310",
        "ipv6_dst":"fe80::120b:a9ff:fef4:ffff",
	"actions":"output=1"
	}
flow1 = {
        'switch':"00:00:00:22:33:44:55:66",
	"name":"flow-mod-1",
	"cookie":"1",
	"priority":"32768",
	"active":"true",
	"ingress-port":"1",
        "eth_type":"0x86DD",
        "ipv6_src":"fe80::120b:a9ff:aaaa:e310",
        "ipv6_dst":"fe80::120b:a9ff:bbbb:ffff",
	"actions":"output=0"
	}


pusher.set(flow0)
pusher.set(flow1)
