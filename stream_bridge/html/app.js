document.addEventListener('DOMContentLoaded', () => {
  const ws = new WebSocket('wss://' + location.host + '/ws');
  ws.onmessage = (event) => {
    console.log('frame', event.data);
  };
});
