import pymongo
import redis
import memcache

client = redis.Redis(host="192.168.1.120", port=32214)
client.flushall()

myclient = pymongo.MongoClient("192.168.1.120:32212")
mydb = myclient["review"]
mycol = mydb["review"]
mycol.drop()

mc = memcache.Client(["192.168.1.120:31211"])
mc.flush_all()