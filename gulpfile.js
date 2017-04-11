var gulp = require('gulp');
var connect = require('gulp-connect');

var paths = {
  bootstrap_js: 'node_modules/bootstrap/dist/js/bootstrap.js',
  bootstrap_css: 'node_modules/bootstrap/dist/css/bootstrap.css',
  jquery: 'node_modules/jquery/dist/jquery.js'
};

gulp.task('bootstrap_js', function() {
  return gulp.src(paths.bootstrap_js).pipe(gulp.dest('data/js'));
});

gulp.task('bootstrap_css', function() {
  return gulp.src(paths.bootstrap_css).pipe(gulp.dest('data/css'));
});

gulp.task('jquery', function() {
  return gulp.src(paths.jquery).pipe(gulp.dest('data/js'));
});


gulp.task('default', ['bootstrap_js', 'bootstrap_css', 'jquery']);

gulp.task('connect', function(){
  connect.server({root: 'data', livereload: true});
});
