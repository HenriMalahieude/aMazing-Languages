package main

type Vector2 struct {
	x int
	y int
}

type cell struct {
	up bool
	down bool
	left bool
	right bool
	visited bool
}

type Maze struct {
	dim int
	Visual [][]string
	gen_stack []Vector2
	gen_stack_top int
}

func NewMaze(d int) *Maze{
	out := &Maze{
		dim: d,
		Visual: make([][]string, d),
		gen_stack: make([]Vector2, d*d),
		gen_stack_top: 0,
	}

	for i := 0; i < d; i++ {
		out.Visual[i] = make([]string, d)
	}

	return out;
}

func (m *Maze) Generate() {

}

func (m Maze) Display() {
	
}