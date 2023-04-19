<template>
  <div class="root-container">
    <button @click="ws_send_str">Socket Interaction</button>
  </div>
</template>

<script setup>

import { useStore } from 'vuex';

const store = useStore();

const ws = store.state.ws;

// setInterval(() => {
//   ws.send("Message Pack from Vue client");
// }, 1000)

const ws_send_str = function () {
  ws.send("hello?");
}

ws.onmessage = function (e) {
  console.log("message from server = ", e.data);
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
</style>
