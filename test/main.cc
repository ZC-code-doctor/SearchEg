#include <sw/redis++/redis++.h>
#include <iostream>

using namespace sw::redis;
using namespace std;

int main() {
    try {
        // 创建 Redis 对象，连接到 Redis 服务器
        auto redis = Redis("tcp://127.0.0.1:6379");

        // 设置 key-value 对
        redis.set("key", "value");

        // 获取 key 的值
        auto val = redis.get("key");
        if (val) {
            // 如果 key 存在
            cout << "key: " << *val << endl;
        } else {
            // 如果 key 不存在
            cout << "key does not exist" << endl;
        }
    } catch (const Error &e) {
        cerr << "Redis error: " << e.what() << endl;
    }

    return 0;
}
