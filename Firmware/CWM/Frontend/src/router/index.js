import Vue from 'vue'
import Router from 'vue-router'
import Winding from '@/components/control/Winding'


Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      name: 'Winding',
      component: Winding
    }
  ]
})
