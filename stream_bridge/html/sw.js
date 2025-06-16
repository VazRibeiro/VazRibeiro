self.addEventListener('install', event => {
  event.waitUntil(caches.open('can-cache').then(c => c.addAll(['./', 'index.html', 'app.js'])));
});
self.addEventListener('fetch', event => {
  event.respondWith(caches.match(event.request).then(resp => resp || fetch(event.request)));
});
