class Motor {
  constructor(x, y) {
    this.x = x
    this.y = y
    this.w = 30
    this.h = 10
    this.p = 0
    this.counter = 0
    this.velDefect = 0
  }

  analogWrite(value) {
    this.p = value
  }

  analogRead(pin) {
    if (pin == 1) return this.#vel()
    return this.x
  }

  state() {
    const { x, y, w, h, counter, p } = this
    const v = this.#vel()
    return { x, y, w, h, counter, v, p }
  }

  setCounter(v) {
    this.counter = v
  }

  getCounter() {
    return Math.floor(this.counter)
  }

  #vel() {
    const v = (this.p / 300) * 100
    return v + this.velDefect
  }

  update() {
    const v = this.#vel()
    this.counter += v * 16
    this.x += v
  }
}
