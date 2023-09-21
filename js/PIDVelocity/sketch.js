function setup() {
  createCanvas(1400, 400)
  drawBackground()
  // frameRate(60)
  arduinoSetup()
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

function printPIDState({ myState, outputSum }, textYLocation) {
  const { Input, Output, SetPoint } = myState
  text('PID SP: ' + Math.floor(SetPoint), 700, textYLocation)
  text('Input: ' + Math.floor(Input), 800, textYLocation)
  text('Onput: ' + Math.floor(Output), 900, textYLocation)
  text('Sum(err): ' + Math.floor(outputSum), 1005, textYLocation)
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
  printPIDState(myPID, 30)
  printPIDState(myPID2, 50)
  updateModel()
}

function downloadCsv(pid) {
  const rows = pid
    .getIterationData()
    .map(({ t, input, output, setPoint, errSum, outMax, outMin }) =>
      [t, input, output, setPoint, errSum, outMax, outMin].join(',')
    )

  const csvContent =
    'data:text/csv;charset=utf-8,' +
    'time,Input,Output,SetPoint,errSum,outMax,outMin\n' +
    rows.join('\n')
  const encodedUri = encodeURI(csvContent)
  window.open(encodedUri)
}
