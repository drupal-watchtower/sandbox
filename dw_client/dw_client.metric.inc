<?php

/**
 * Parses size strings
 *
 * @param $size_str string - formatted size string like "128M" or "32k"
 *
 * @return int
 */
function _return_bytes($size_str) {
  switch (substr($size_str, -1)) {
    case 'M':
    case 'm':
      return (int) $size_str * 1048576;
    case 'K':
    case 'k':
      return (int) $size_str * 1024;
    case 'G':
    case 'g':
      return (int) $size_str * 1073741824;
    default:
      return $size_str;
  }
}

function dw_client_greatest_user_id() {
  return db_query("SELECT MAX(uid) FROM {users}")->fetchField();
}

function dw_client_greatest_watchdog_id() {
  return db_query("SELECT MAX(wid) FROM {watchdog}")->fetchField();
}
