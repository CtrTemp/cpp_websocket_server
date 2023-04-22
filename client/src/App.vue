<template>
  <div class="root-container">
    <button @click="ws_client_send_wrong_message">Socket Err Test</button>
    <button @click="ws_client_close_server">Socket Close</button>
    <button @click="ws_client_fetch_json_str">Socket Get Json</button>
    <button @click="ws_client_send_json_str">Socket Send Json</button>

  </div>
</template>

<script setup>

import { useStore } from 'vuex';

const store = useStore();

const ws = store.state.ws;


const ws_client_send_wrong_message = function () {
  const json_pack = {
    hah: "haah"
  };
  ws.send(JSON.stringify(json_pack));
  // ws.send(json_pack);
}

// 发送消息停掉后台
const ws_client_close_server = function () {

  const json_pack = {
    cmd: "close"
  }
  ws.send(JSON.stringify(json_pack));
}


// 发送消息停 从后台得到json数据
const ws_client_fetch_json_str = function () {

  const json_pack = {
    cmd: "get_json"
  }
  ws.send(JSON.stringify(json_pack));
}

// 发送 json 到后端
const ws_client_send_json_str = function () {
  const json_pack = {
    cmd: "test",
    name: "CtrTemp",
    prj: "Rendering",
    age: 24
  }

  ws.send(JSON.stringify(json_pack));
}


ws.onmessage = function (e) {
  const json_data_pack = JSON.parse(e.data);
  console.log("json_data_pack from server = ", json_data_pack);
}

</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}

.root-container {
  display: flex;
  flex-direction: column;

  justify-content: center;
  align-items: center;

  gap: 1vh;
}
</style>
