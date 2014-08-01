<?php
/**
 * Haml view renderer class file.
 * Renders {@link http://haml-lang.com/ HAML} view files using
 * {@link http://phamlp.googlecode.com PHamlP}.
 * Please see the {@link documentation http://haml-lang.com/docs/yardoc/file.HAML_REFERENCE.html#plain_text HAML} for the syntax.
 * 
 * * To use the Haml view renderer:
 * * Put {@link http://phamlp.googlecode.com PHamlP} in your vendors directory
 * * Put this file in your views directory
 * * Configure the view and HamlParser using
 *   <pre>Configure::write('Haml.<optionName>', <optionValue>);</pre>
 *   See below and HamlParser for a description of options
 * * Add the following line in your app_controller.php
 *   <pre>var $view = 'Haml';</pre>
 * * Create .haml views in your view folders
 * 
 * By default the parsed views (.ctp files) are cached under
 * <pre>APP.tmp.haml</pre>
 *
 * @author			Chris Yates <chris.l.yates@gmail.com>
 * @copyright		Copyright &copy; 2010 PBM Web Development
 * @author			Chris Cinelli <chris.cinelli@formativelearning.com>
 * @copyright		Copyright &copy; 2011 - Patch for Cake 2.0  
 * @license			http://phamlp.googlecode.com/files/license.txt
 * @package			PHamlP
 * @subpackage	Cake
 */
/**
 * Haml allows you to write view files in {@link Haml http://haml-lang.com/} and
 * render them using {@link http://phamlp.googlecode.com PHamlP}.
 * 
 * If a .haml view is not found a .ctp or .thtml view is used.
 * @package			PHamlP
 * @subpackage	Cake
 */
class HamlView extends View {
	/**#@+
	 * Configurable Options
	 */
	/**
	 * @var string the extension name of the view file. Defaults to '.haml'.
	 */
	var $ext;
	/**
	 * @var boolean whether to use Cake's cache directory. If false the view file
	 * is created in the view directory. Defaults to true.
	 */
	var $useCachePath;
	/**
	 * @var integer the chmod permission for temporary directories and files
	 * generated during parsing. Defaults to 0755 (owner rwx, group rx and others rx).
	 */
	var $filePermission;
	/**
	 * @var boolean whether to cache parsed files. If false files will be parsed
	 * every time.
	 */
	var $cache;
	/**
	 * @var array default option values.
	 */
	var $defaults = array('ext'=>'.haml','useCachePath'=>true,'filePermission'=>0755,'cache'=>false);
	/**#@-*/

	/**
	* @var HamlParser the Haml parser
	*/
	private $haml;
	/**
	 * @var array Haml parser option names. These are passed to the parser if set.
	 * 
	 * format: string DOCTYPE format
	 * 
	 * doctype: string custom doctype. If null (default) {@link format} must be
	 * a key in {@link doctypes}
	 * 
	 * escapeHtml: boolean whether or not to escape X(HT)ML-sensitive characters 
	 * in script. If this is true, = behaves like &=; otherwise, it behaves like
	 * !=. Note that if this is set, != should be used for yielding to
	 * subtemplates and rendering partials.
	 * Defaults to false.
	 * 
   * suppressEval: boolean Whether or not attribute hashes and scripts
   * designated by = or ~ should be evaluated. If true, the scripts are rendered
   * as empty strings.
   * Defaults to false.
   * 
	 * attrWrapper: string The character that should wrap element attributes.
	 * Characters of this type within attributes will be escaped (e.g. by
	 * replacing them with &apos;) if the character is an apostrophe or a
	 * quotation mark.
	 * Defaults to " (an quotation mark).
	 *
	 * style: string style of output. Can be:
	 * nested: output is nested according to the indent level in the source
	 * expanded: block tags have their own lines as does content which is indented
	 * compact: block tags and their content go on one line
	 * compressed: all unneccessary whitepaces is removed. If ugly is true this
	 * style is used.
	 * 
	 * ugly: boolean if true no attempt is made to properly indent or format
	 * the output. Reduces size of output file but is not very readable.
	 * Defaults to true.
	 * 
	 * preserveComments: boolean if true comments are preserved in ugly mode.
	 * If not in ugly mode comments are always output.
	 * Defaults to false.
	 * 
	 * debug: integer Initial debug setting:
	 * no debug, show source, show output, or show all.
	 * Debug settings can be controlled in the template
	 * Defaults to DEBUG_NONE.
	 * 
	 * filterDir: string Path to filters. If specified this will be searched
	 * first followed by 'vendors.phmlp.haml.filters'.
	 * This allows the default filters to be overridden.
	 * 
	 * doctypes: array supported doctypes
	 * See HamlRender for default
	 * 
	 * emptyTags: array A list of tag names that should be automatically
	 * self-closed if they have no content.
	 * See HamlRender for default
	 * 
	 * inlineTags: array A list of inline tags for which whitespace is not collapsed
	 * fully when in ugly mode or stripping outer whitespace.
	 * See HamlRender for default
	 * 
	 * minimizedAttributes: array attributes that are minimised.
	 * See HamlRender for default
	 * 
	 * preserve: array A list of tag names that should automatically have their
	 * newlines preserved.
	 */
	var $hamlOptions = array('format', 'doctype', 'escapeHtml', 'suppressEval', 'attrWrapper', 'style', 'ugly', 'preserveComments', 'debug', 'filterDir', 'doctypes', 'emptyTags', 'inlineTags', 'minimizedAttributes', 'preserve', 'helperFile');
	
	/**
	 * Initialises HamlView. 
	 * @param Controller $controller
	 * @return HamlView
	 */
	function __construct(&$controller) {
		parent::__construct($controller);		
		foreach ($this->defaults as $key=>$value) {
			$option = Configure::read("Haml.$key");
			$this->$key = (is_null($option)?$value:$option);
		} // foreach
	}

	/**
	 * Do a sanity check on the options and setup alias to filters
	 */
	private function _init() {
		$options = array();
		foreach ($this->hamlOptions as $option) {
			$_option = Configure::read("Haml.$option");
			if (!is_null($_option)) {
				$options[$option] = $_option;
			}
		} // foreach

		App::import('Vendor', 'HamlParser', array('file'=>'phamlp'.DS.'haml'.DS.'HamlParser.php'));
		$this->haml = new HamlParser($options);
    }
    
	/**
	 * Renders and returns output for given view filename with its
	 * array of data.
	 *
	 * @param string Filename of the view
	 * @param array Data to include in rendered view
	 * @return string Rendered output
	 * @access protected 
	 */
	function _render($___viewFn, $___dataForView = array()) {
		if (substr($___viewFn, strrpos($___viewFn, '.')) == $this->ext) {
			$cachedViewFile = $this->_getCachedViewFileName($___viewFn);
			if(!$this->cache||@filemtime($___viewFn)>@filemtime($cachedViewFile)) {
				if (empty($this->haml)) $this->_init();
				file_put_contents($cachedViewFile, $data = $this->haml->parse($___viewFn));
				@chmod($cachedViewFile, $this->filePermission);
			}
			return parent::_render($cachedViewFile, $___dataForView);
		}
		else {
			return parent::_render($___viewFn, $___dataForView);
		}
	}

	/**
	 * Generates the cached view file path.
	 * @param string source view file path
	 * @return string cached view file path
	 * @access private
	 */
	private function _getCachedViewFileName($file)	{
		$cachedViewFile = str_replace(substr($file, strrpos($file, '.')), '.ctp', $file);
		if($this->useCachePath)	{
			$cachedViewFile = str_replace(APP.'View'.DS, CACHE.'haml'.DS, $cachedViewFile);
			if(!is_file($cachedViewFile))
				@mkdir(dirname($cachedViewFile),$this->filePermission,true);
		}
		return $cachedViewFile;
	}
}