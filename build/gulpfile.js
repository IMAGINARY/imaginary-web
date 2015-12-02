'use strict';

var imaginary3_theme_path = '../sites/all/themes/imaginary3';

var gulp = require('gulp');
var sass = require('gulp-sass');
var sourcemaps = require('gulp-sourcemaps');

gulp.task('sass', function () {
  gulp.src(imaginary3_theme_path + '/sass/**/*.scss')
      .pipe(sourcemaps.init())
      .pipe(sass().on('error', sass.logError))
      .pipe(sourcemaps.write('.'))
      .pipe(gulp.dest(imaginary3_theme_path + '/styles'));
});

gulp.task('sass:watch', function () {
  gulp.watch(imaginary3_theme_path + '/sass/**/*.scss', ['sass']);
});

gulp.task('default', ['sass']);