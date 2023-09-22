const d = {
  motor: new Motor(50, 200),
  motor2: new Motor(50, 300),
  startTime: 0,
}

function setup() {
  createCanvas(1500, 500)
  drawBackground()
  // frameRate(5)
  d.motor.analogWrite(100)
  d.motor2.analogWrite(150)
  startTime = millis()
}

function drawBackground() {
  background(150, 100, 50)
  line(50, 0, 50, 500)
  line(500, 0, 500, 500)
  line(1000, 0, 1000, 500)
}

function drawMotor({ x, y, w, h }) {
  const x0 = x - w
  const y0 = y - h / 2
  const y2 = y + h / 2

  line(x, y, x0, y0)
  line(x0, y0, x0, y2)
  line(x0, y2, x, y)
}

function drawState({ x, v, counter, p }, textYLocation) {
  text('P: ' + Math.floor(p), 50, textYLocation)
  text('V: ' + Math.floor(v * 10) / 10, 100, textYLocation)
  text('X: ' + Math.floor(x), 200, textYLocation)
  text('C: ' + Math.floor(counter), 300, textYLocation)
  text('time: ' + Math.floor(millis()), 500, textYLocation)
}

function updateModel() {
  d.motor.update()
  d.motor2.update()
}

function draw() {
  const motor1State = d.motor.state()
  const motor2State = d.motor2.state()
  drawBackground()
  drawMotor(motor1State)
  drawMotor(motor2State)
  drawState(motor1State, 30)
  drawState(motor2State, 50)

  const now = millis()
  if (now - startTime < 1000) {
    d.motor.analogWrite(0)
  } else if (now - startTime < 5000) {
    d.motor.analogWrite(250)
  } else if (now - startTime < 27000) {
    d.motor.analogWrite(0)
  }

  updateModel()
}
