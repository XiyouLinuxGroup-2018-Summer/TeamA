addMission(json) {
    // 解析sendID, 上在线用户锁->查看是否在线->解锁
    // 则加入sendQue 或者 offlineQue
}

sendOfflineQue() {
    // 查找当前队列中有无sendID符合条件的
    // 有 调用sendData()
}

sendData() {
    while(<上锁>!queue().empty) {
        // 创建副本
        // int len = sizeof(json)
        // send()
        // 如果send的字符数小于len
        // 则重新入队列
    }<解锁>
}
