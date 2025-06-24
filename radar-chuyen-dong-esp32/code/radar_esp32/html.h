const char MAIN_page[] PROGMEM = R"RawString(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Radar 360°</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#0a0a0a;font-family:monospace;color:#0f0;overflow:hidden;height:100vh;display:flex;flex-direction:column;align-items:center;padding:10px}
.title{font-size:1.8rem;text-align:center;margin:10px 0;text-shadow:0 0 10px #0f0}
.info{display:flex;gap:20px;margin:10px 0;font-size:1.1rem;flex-wrap:wrap;justify-content:center}
.info div{padding:8px 15px;border:1px solid #0f0;border-radius:5px;background:rgba(0,255,0,0.1)}
.controls{display:flex;gap:15px;margin:10px 0;flex-wrap:wrap;justify-content:center}
.btn{padding:8px 20px;border:1px solid #0f0;background:rgba(0,255,0,0.1);color:#0f0;border-radius:5px;cursor:pointer;font-family:monospace;font-size:0.9rem;transition:all 0.3s}
.btn:hover{background:rgba(0,255,0,0.2);box-shadow:0 0 10px rgba(0,255,0,0.5)}
.btn:active{transform:scale(0.95)}
.btn.clear{border-color:#ff4444;color:#ff4444;background:rgba(255,68,68,0.1)}
.btn.clear:hover{background:rgba(255,68,68,0.2);box-shadow:0 0 10px rgba(255,68,68,0.5)}
.radar{width:350px;height:350px;border:2px solid #0f0;border-radius:50%;position:relative;background:radial-gradient(circle,rgba(0,255,0,0.05),rgba(0,0,0,0.9));margin:10px}
.grid{position:absolute;top:50%;left:50%;transform:translate(-50%,-50%);width:100%;height:100%}
.circle{position:absolute;border:1px solid rgba(0,255,0,0.3);border-radius:50%;top:50%;left:50%;transform:translate(-50%,-50%)}
.line{position:absolute;background:rgba(0,255,0,0.2);top:50%;left:50%;width:175px;height:1px;transform-origin:0 0}
.sweep{position:absolute;top:50%;left:50%;width:175px;height:2px;background:linear-gradient(90deg,#0f0,transparent);transform-origin:0 0;transition:transform 0.1s}
.dot{position:absolute;width:6px;height:6px;background:#f44;border-radius:50%;animation:blink 1s infinite}
.dot.old{background:#ff8800;animation:fadeOut 1s ease-out}
.labels{position:absolute;top:0;left:0;width:100%;height:100%;pointer-events:none;font-size:10px}
.label{position:absolute;color:#0f0}
.stats{font-size:0.8rem;margin:5px 0;opacity:0.7}
@keyframes blink{0%,100%{opacity:1}50%{opacity:0.3}}
@keyframes fadeOut{0%{opacity:1;transform:scale(1)}100%{opacity:0.2;transform:scale(0.8)}}
@media(max-width:480px){
  .radar{width:280px;height:280px}
  .info{flex-direction:column;gap:10px}
  .title{font-size:1.4rem}
  .controls{flex-direction:column}
  .btn{width:100%;max-width:200px}
}
</style>
</head>
<body>
<h1 class="title">RADAR 360°</h1>
<div class="info">
<div>DIST: <span id="d">0</span>cm</div>
<div>ANGLE: <span id="a">0</span>°</div>
<div>STATUS: <span id="s">SCAN</span></div>
<div class="stats">OBJECTS: <span id="objCount">0</span></div>
</div>
<div class="controls">
<button class="btn" onclick="toggleAutoClean()">AUTO CLEAN: <span id="autoStatus">ON</span></button>
<button class="btn clear" onclick="clearOldObjects()">CLEAR OLD (>3s)</button>
<button class="btn clear" onclick="clearAllObjects()">CLEAR ALL</button>
</div>
<div class="radar" id="radar">
<div class="grid">
<div class="circle" style="width:20%;height:20%"></div>
<div class="circle" style="width:40%;height:40%"></div>
<div class="circle" style="width:60%;height:60%"></div>
<div class="circle" style="width:80%;height:80%"></div>
</div>
<div class="line" style="transform:rotate(0deg)"></div>
<div class="line" style="transform:rotate(45deg)"></div>
<div class="line" style="transform:rotate(90deg)"></div>
<div class="line" style="transform:rotate(135deg)"></div>
<div class="line" style="transform:rotate(180deg)"></div>
<div class="line" style="transform:rotate(225deg)"></div>
<div class="line" style="transform:rotate(270deg)"></div>
<div class="line" style="transform:rotate(315deg)"></div>
<div class="sweep" id="sweep"></div>
<div class="labels">
<div class="label" style="top:5px;left:50%;transform:translateX(-50%)">0°</div>
<div class="label" style="top:50%;right:5px;transform:translateY(-50%)">90°</div>
<div class="label" style="bottom:5px;left:50%;transform:translateX(-50%)">180°</div>
<div class="label" style="top:50%;left:5px;transform:translateY(-50%)">270°</div>
</div>
</div>
<script>
let objs = [];
let autoClean = true;
let lastAngle = 0;

function update(){
  fetch('/readDistance').then(r=>r.json()).then(d=>{
    let dist = +d[0], ang = +d[1];
    
    // Xử lý góc để hiển thị 0-360°
    if (ang < 0) ang += 360;
    if (ang > 360) ang -= 360;
    
    document.getElementById('d').textContent = dist;
    document.getElementById('a').textContent = Math.round(ang);
    document.getElementById('s').textContent = dist < 50 ? 'DETECT' : 'SCAN';
    document.getElementById('sweep').style.transform = `rotate(${ang}deg)`;
    
    if(dist < 50 && dist > 2) addObj(ang, dist);
    
    lastAngle = ang;
  }).catch(() => document.getElementById('s').textContent = 'ERROR');
}

function addObj(a, d){
  // Loại bỏ vật thể trùng lặp (cùng vị trí)
  objs = objs.filter(o => Math.abs(o.a - a) > 10 || Math.abs(o.d - d) > 10);
  
  objs.push({a, d, t: Date.now()});
  
  // Giới hạn số lượng vật thể
  if(objs.length > 20) objs.shift();
  
  updateStats();
  render();
}

function clearOldObjects(){
  let now = Date.now();
  let oldCount = objs.length;
  objs = objs.filter(o => now - o.t < 3000); // Giữ lại vật thể < 3s
  
  if(oldCount > objs.length){
    console.log(`Cleared ${oldCount - objs.length} old objects`);
  }
  
  updateStats();
  render();
}

function clearAllObjects(){
  objs = [];
  updateStats();
  render();
  console.log('All objects cleared');
}

function toggleAutoClean(){
  autoClean = !autoClean;
  document.getElementById('autoStatus').textContent = autoClean ? 'ON' : 'OFF';
  console.log('Auto clean:', autoClean);
}

function updateStats(){
  document.getElementById('objCount').textContent = objs.length;
}

function render(){
  // Xóa tất cả dots cũ
  document.querySelectorAll('.dot').forEach(e => e.remove());
  
  let now = Date.now();
  
  objs.forEach(o => {
    let dot = document.createElement('div');
    let age = now - o.t;
    
    // Tính toán vị trí dựa trên góc 0-360°
    let r = Math.min(o.d / 50 * 175, 175); // Giới hạn bán kính
    let rad = o.a * Math.PI / 180;
    let x = 175 + r * Math.sin(rad);
    let y = 175 - r * Math.cos(rad);
    
    dot.className = age > 3000 ? 'dot old' : 'dot';
    dot.style.left = x + 'px';
    dot.style.top = y + 'px';
    
    // Độ mờ dần theo thời gian
    let opacity = Math.max(0.2, 1 - age / 8000);
    dot.style.opacity = opacity;
    
    // Tooltip hiển thị thông tin
    dot.title = `${Math.round(o.d)}cm @ ${Math.round(o.a)}° (${Math.round(age/1000)}s ago)`;
    
    document.getElementById('radar').appendChild(dot);
  });
}

function autoCleanObjects(){
  if(autoClean){
    let now = Date.now();
    let oldCount = objs.length;
    objs = objs.filter(o => now - o.t < 8000); // Auto xóa sau 8s
    
    if(oldCount > objs.length){
      updateStats();
    }
  }
}

// Intervals
setInterval(update, 100);           // Cập nhật dữ liệu mỗi 100ms
setInterval(render, 500);           // Render lại mỗi 500ms
setInterval(autoCleanObjects, 2000); // Auto clean mỗi 2s
setInterval(updateStats, 1000);     // Cập nhật stats mỗi 1s

// Khởi tạo
update();
updateStats();

// Keyboard shortcuts
document.addEventListener('keydown', function(e){
  switch(e.key.toLowerCase()){
    case 'c': clearOldObjects(); break;
    case 'x': clearAllObjects(); break;
    case 'a': toggleAutoClean(); break;
  }
});

console.log('Radar system loaded. Shortcuts: C=Clear Old, X=Clear All, A=Toggle Auto Clean');
</script>
</body>
</html>
)RawString";