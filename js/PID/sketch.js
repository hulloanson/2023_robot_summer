const d = {
  motor: new Motor(50, 200),
  motor2: new Motor(50, 300),
}

function setup() {
  createCanvas(1000, 400)
  drawBackground()
  // frameRate(60)
  arduinoSetup()
}

function drawBackground() {
  background(150, 100, 50)
  line(800, 0, 800, 400)
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

function printPIDState({ Input, Output }, textYLocation) {
  text('PID Input: ' + Math.floor(Input), 600, textYLocation)
  text('Onput: ' + Math.floor(Output), 700, textYLocation)
}

function updateModel() {
  d.motor.update()
  d.motor2.update()
}

function draw() {
  arduinoLoop()
  const motor1State = d.motor.state()
  const motor2State = d.motor2.state()
  drawBackground()
  drawMotor(motor1State)
  drawMotor(motor2State)
  drawState(motor1State, 30)
  drawState(motor2State, 50)
  printPIDState(myPID.myState, 30)
  printPIDState(myPID2.myState, 50)
  updateModel()
}
