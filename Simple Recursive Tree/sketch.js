let treeLength = 100;
let angle;
let lengthFactor = 0.7;
let AngleSlider;
function setup() {
  createCanvas(400, 600);
  AngleSlider = createSlider(0,45,30,0.1);
}

function drawTree(length,ang){

  line(0,0,length,0);
  stroke(0,length+50,30);
  strokeWeight(length*0.1);
  translate(length,0); 
  rotate(ang);
 
  if( length > 2 ){
    push(); 
    drawTree(length*lengthFactor,angle);  
    pop();
    push();
    drawTree(length*lengthFactor,-angle);
    pop();
  }
}

function draw() {
  background(220);
  angle = map(AngleSlider.value(),0,180,0,TWO_PI);
  translate(width/2,height);
  rotate(-PI/2);
  drawTree(treeLength,0);
}