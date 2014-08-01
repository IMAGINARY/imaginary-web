<?php
/**
 * @file
 * View template to display a delimited list of text rows.
 *
 * @ingroup views_templates
 */

// Prepare variables
$count = count($rows);
$separator = ($count == 2 && $count != $options['long_count']) ? $options['separator_two'] : $options['separator_long'];
$last_delimiter = $separator == 'both' || $separator == 'delimiter';
$last_conjunctive = $separator == 'both' || $separator == 'conjunctive';

?>
<?php if (!empty($title)): ?>
<h3><?php print $title; ?></h3>
<?php endif; ?>
<div class="views-delimited-list">
<?php
// Prefix
if (!empty($options['prefix'])) {
  print '<span class="views-delimited-list-prefix">';
  print $options['prefix'];
  print '</span>';
}

foreach ($rows as $i => $row) {
  $index = $i + 1;

  // Row
  print '<span class="' . $classes_array[$i] . '">';
  print trim($row);
  print '</span>';

  // Delimiter
  if ($index < $count - 1 || $index == $count - 1 && $last_delimiter) {
    print '<span class="views-row-delimiter">' . $options['delimiter'] . '</span>';
  }

  // Conjunctive
  if ($index == $count - 1 && $last_conjunctive && is_string($options['conjunctive']) && $options['conjunctive'] !== '') {
    print '<span class="views-row-conjunctive">' . $options['conjunctive'] . '</span>';
  }
}

// Suffix
if (!empty($options['suffix'])) {
  print '<span class="views-delimited-list-suffix">';
  print $options['suffix'];
  print '</span>';
}
?>
</div>