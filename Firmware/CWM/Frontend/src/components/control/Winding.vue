<template>
  <div id="content">
    <div id="preview">
      <div class="previewControls">
        <button id="apply" class="btn btn-primary btn-md" v-on:click="start()" role="button">Apply</button>
      </div>
    </div>
  </div>
</template>

<script>
import Vue from 'vue'
import axios from 'axios'
import toastr from 'toastr'

export default {
  name: 'Winding',
  onMounted () {
    setInterval(this.checkActive(), 500);
  },
  data () {
    return {
      active: false,
      speed: 500,
      gauge: 0.3,
      numberOfWindings: 400
    }
  },
  methods: {
    start: function () {
      if (this.active) {
        toastr.error('Winding in progress\nPlease wait until current job is finished ', 'Error')
        return;
      }

      let payload = {
        "speed" : this.speed,
        "gauge" : this.gauge,
        "count" : this.numberOfWindings
      };

      axios.post(
        '/windCoil',
        {data: JSON.stringify(payload)},
        {headers: { 'Content-Type': 'multipart/form-data' }
        }).then(function (resp) {
        console.log(resp)
        toastr.success('Starting...', 'Success')
      }).catch(function (error) {
        console.log(error)
        toastr.error('Something went wrong!', 'Error')
      })
    },
    checkActive: function (percentPerSeg) {
      let vm = this;

      axios.get('/getStatus').then(function (resp) {
        vm.active = resp;
        console.log(resp)
      }).catch(function (error) {
        console.log(error)
        toastr.error('Something went wrong!', 'Error')
      })
    },
  }
}

</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
#content {
    display:grid !important;
    overflow:hidden;
}

.center {
  margin: 0 auto;
}

.bottom-nav-item {
  width:50px;
  height:50px
}

.removeColor {
    height: 20px;
    width: 20px;
    background: #444444;
    border-radius: 50%;
    display: flex;
    align-content: center;
    justify-content: center;
    position: relative;
    align-items: center;
    top: 23px;
    left: 29px;
    cursor:pointer;
}

.outer-ring {
  left: calc(50% - 150px);
  height: 300px;
  width: 300px;
  background-image: linear-gradient(0deg, #000 0%,#000 100%);
  /*linear-gradient(0deg, #FEED07 0%, #FE6A50 5%, #ED00AA 15%, #2FE3FE 50%, #8900FF 100%);*/
  border-radius: 50%;
  /*  Rotate  */
  animation-duration: 2s;
  animation-name: rotate;
  animation-iteration-count: infinite;
}

.inner-ring {
    position: relative;
    /* left: calc(50% - 140px); */
    height: 280px;
    width: 280px;
    background-image: -webkit-gradient(linear, left bottom, left top, from(#36295e), to(#1c1045));
    background-image: linear-gradient(0deg, #36295e, #1c1045);
    border-radius: 50%;
    top: -290px;
}

h1, h2 {
  font-weight: normal;
}
ul {
  list-style-type: none;
  padding: 0;
}
li {
  display: inline-block;
  margin: 0 10px;
}

#bottom-nav {
  background: #212121 !important;
  color: white !important;
  height: 50px;
  position: absolute;
  bottom: 10px;
  max-width: 300px;
  width: 300px;
  left: calc(50% - 150px);
}
/* Chrome, Safari, Edge, Opera */
input::-webkit-outer-spin-button,
input::-webkit-inner-spin-button {
  -webkit-appearance: none;
  margin: 0;
}

/* Firefox */
input[type=number] {
  -moz-appearance: textfield;
}

button:focus {outline:0;}

select:focus {outline:0;}

#generate {
    background: darkslateblue;
    border: none;
    border-radius: 5px;
    box-shadow:4px 5px 10px #000;
    -moz-box-shadow: 4px 5px 10px #000;
    -webkit-box-shadow: 4px 5px 10px #000;
}

#apply {
    background: darkslateblue;
    border: none;
    border-radius: 5px;
    margin-left: 10px;
    box-shadow:4px 5px 10px #000;
    -moz-box-shadow: 4px 5px 10px #000;
    -webkit-box-shadow: 4px 5px 10px #000;
}

button:hover {
  background-color: #2914a9 !important;
  border-color: #2914a9 !important;
  transition: background-color .3;
}

select {
  height: 40px;
  font-size: 17px;
  margin-left: 10px;
  border-color:#212121;
  -webkit-box-shadow: 4px 5px 10px #000;
}

#actions {
    position: relative;
    top: -270px;
}

#addColor {
    color: #fff !important;
    background-color: #c200f9 !important;
    border-color: #c200f9 !important;
    border-radius: 50% !important;
    position: fixed !important;
    bottom: 25px !important;
    z-index:100;
    right: 15px !important;
    box-shadow:4px 5px 10px #000;
    -moz-box-shadow: 4px 5px 10px #000;
    -webkit-box-shadow: 4px 5px 10px #000;
}

#addColor:hover {
  background-color: #6600f9 !important;
  border-color: #6600f9 !important;
  transition: background-color .3;
}

#colorSelector {
    display: inline-flex;
    justify-content: center;
    max-height: 115px;
    max-width: 350px;
    overflow-y: scroll;
    padding-right: 17px;
    box-sizing: content-box;
    flex-wrap: wrap;
    height: 115px;
    width: 350px;
    background: #111;
    border-radius: 15px;
}

#colorSelector::-webkit-scrollbar {
    display: none;  /* Safari and Chrome */
}

#preview {
  padding-top: 25px;
  display: inline-block;
  width:100%;
  max-height:400px;
}

.previewControls {
  max-height: 450px;
}

.main {
  justify-content: center;
  display: inline-grid;
}

.colorHolder {
  display: flex;
  justify-content: center;
  margin-top: 5px;
  border-radius:2px;
}

input[type=color]  {
    background: transparent !important;
    border: none;
}

input {
  max-width: 50px !important;
}

.nav-link {
  filter: invert(42%) sepia(93%) saturate(1352%) hue-rotate(266deg) brightness(119%) contrast(119%);
  height:50px;
  padding:0;
  width:50px;
  margin-left:5px;
  display:flex;
  justify-content:center;
  align-items: center;
}

svg {
  height:25px;
}

.color {
  height: 28px;
  width: 28px;
  margin: auto;
  position:relative;
  border-radius:2px;
  cursor: pointer;
  border:none !important;
}

.additionalColor{
  color:white;
  font-weight:bold;
  text-align: center;
  margin: 10px 10px;
  height:fit-content;
}

.switch {
  position: relative;
  display: inline-block;
  width: 50px;
  height: 25px;
  margin: 30px;
}

.switch input {
  opacity: 0;
  width: 0;
  height: 0;
}

.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #ccc;
  -webkit-transition: 0.4s;
  transition: 0.4s;
}

.slider:before {
  position: absolute;
  content: "";
  height: 20px;
  width: 20px;
  left: 0px;
  bottom: 2px;
  background-color: white;
  -webkit-transition: 0.4s;
  transition: 0.4s;
}

input:checked + .icon::before {
  font-family: "FontAwesome";
  content: "\f00c";
  transform: rotate(45deg);
  padding-top: 5px;
}

.toggle {
  font-family: "FontAwesome";
  font-style: normal !important;
}

input + .icon::before {
  font-family: "FontAwesome";
  content: "\f00d";
  padding-top: 5px;
}

.slider.w-color:before {
  background-color: #a37eba;
}

.slider.w-color {
  background-color: #ccc;
  -webkit-transition: 0.4s;
  transition: 0.4s;
}

input:checked + .slider.w-color {
  background-color: #ccc;
}

input:checked + .slider.icon {
  background-color: #a37eba;
}

.slider.icon:before {
  background-color: #ffe05d;
}
.slider.icon:after {
  background-color: #e2df23;
}

input:checked + .slider {
  background-color: #2196f3;
}

input:checked + .slider:before {
  -webkit-transform: translateX(26px);
  -ms-transform: translateX(26px);
  transform: translateX(26px);
}

/* Rounded sliders */
.slider.round {
  border-radius: 34px;
}

.slider.round:before {
  border-radius: 50%;
}
.disabled-toggle {
  opacity: 0.5;
  cursor: default;
}
</style>
