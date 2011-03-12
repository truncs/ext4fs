<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN"
"http://www.w3.org/TR/REC-html40/loose.dtd">
<Html>
<Head>
<Title>P4Web.FreeBSD.org - //depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c</Title>
<script type="text/javascript"><!--
var finm
var args
var mShw = 0
var m
function hideMenu() {
	if (m)
		m.style.display='none'
}
function formFieldClear( field, text )
{
	if (field.value == text) {
		field.value = '';
		field.style.color = '#000000';
    }
}
function formFieldRestore( field, text )
{
	if (field.value == '') {
		field.value = text;
		field.style.color = '#A0A0A0';
    }
}
function hideDiv(divname) {
	d = document.getElementById(divname)
	if (d)
		d.style.display='none'
}
function showDiv(divname, divloc) {
	d = document.getElementById(divname)
	if (!d)
		return
	i = document.getElementById(divloc)
	if (!i)
		return
	var t = i.offsetTop
	var p = i.offsetParent
	var l = i.offsetLeft
	var w = i.offsetWidth
	while (p.tagName != 'BODY') {
		t = t + p.offsetTop
		l = l + p.offsetLeft
		p = p.offsetParent
	}
	d.style.top = t + i.offsetHeight + 10
	d.style.left = l+(w/2)
	d.style.display=''
}
function validateFilelogForm() {
	var n = document.forms.filelogForm.revs;
	for(var i=0; i < n.length; i++) {
        	if (n[i].checked) {
				if (document.forms.filelogForm.rev2.value == n[i].value) {
					alert('You attempted to compare a file with itself. Before choosing the "Diff vs. Selected Revision" option, you must select one of the other revisions by clicking its radio button.')
					return false;
				}
				return true;
			}
	}
	alert('Before choosing the "Diff vs. Selected Revision" option, you must select a revision by clicking its radio button.');
	return false;
}
function showMenu(filename, id, menu, show, arg) {
	if (m)
		m.style.display='none'
	m = document.getElementById('menu_'+menu)
	if (!m)
		return
	finm = filename
	if (mShw && arg == '')
		args = '&mu='+mShw
	else
		args = arg
	var n
	var o
	var i
	var j = -1
	while (j < 31) {
		n = o = 1 << ++j
		while (1) {
			i = document.getElementById('id_mu'+menu+o)
			if (i) {
				if (show & n)
				{
					i.style.display=''
					h = document.getElementById('id_mu'+menu+o+'h')
					if (h)
						h.style.display='none'
				}
				else
				{
					i.style.display='none'
				}
				if (n < 4)
					break;
				o++
			}
			else
				break
		}
	}
	i = document.getElementById('id_'+id)
	var t = i.offsetTop
	var p = i.offsetParent
	var l = i.offsetLeft
	while (p.tagName != 'BODY') {
		t = t + p.offsetTop
		l = l + p.offsetLeft
		p = p.offsetParent
	}
	m.style.top = t + i.offsetHeight + 1
	m.style.left = l - 8
	m.style.display=''
}
function setmushow(show) {
	mShw = show
}
function runcmd(cmd) {
	var url = finm + "?ac=" + cmd + args;
	window.location = url;
}
function runurl(url) {
	window.location = url;
}
function runuXc(url, cmd) {
	var newurl = url + finm + cmd;
	window.location = newurl;
}
function promptuXc(str, url, cmd) {
	var val=prompt(str, '')
	if (val!=null && val!='')
	{
		var newurl = url + val
		runuXc(newurl, cmd)
	}
}
function showhi(id)
{
	document.getElementById(id + 'p').style.display='none'
	document.getElementById(id + 'h').style.display=''
}
function hidehi(id)
{
	document.getElementById(id + 'p').style.display=''
	document.getElementById(id + 'h').style.display='none'
}
function check4ESC(e) {
	if (m) {
		var kC  = (window.event) ? event.keyCode : e.keyCode;
		var Esc = (window.event) ? 27 : e.DOM_VK_ESCAPE
	      	if (kC==Esc) {
			m.style.display='none'
			m = 0
		}
	}
        return true;
}
document.onkeyup = check4ESC;
//--></script>
  <Style type="text/css"><!--
  A { text-decoration: none }
  A:link { text-decoration: none; color: #0000CC; font-family: verdana,arial,helvetica,sans-serif; font-size: 12px }
  A:visited { text-decoration: none; color: #0000CC; font-family: verdana,arial,helvetica,sans-serif; font-size: 12px }
  A:hover { text-decoration: underline; color: #0000CC; font-family: verdana,arial,helvetica,sans-serif; font-size: 12px }
  A.status:link { text-decoration: none; color: #FFFFFF; font-family: verdana,arial,helvetica,sans-serif; font-size: 11px }
  A.status:visited { text-decoration: none; color: #FFFFFF; font-family: verdana,arial,helvetica,sans-serif; font-size: 11px }
  A.status:hover { text-decoration: underline; color: #FFFFFF font-family: verdana,arial,helvetica,sans-serif; font-size: 11px }
  A.nav_link:link { text-decoration: none; color: #0000CC; font-family: verdana,arial,helvetica,sans-serif; font-size: 11px }
  A.nav_link:visited { text-decoration: none; color: #0000CC; font-family: verdana,arial,helvetica,sans-serif; font-size: 11px }
  A.nav_link:hover { text-decoration: underline; color: #0000CC; font-family: verdana,arial,helvetica,sans-serif; font-size: 11px }
  A.tab:link { text-decoration: none; color: #000000; font-family: verdana,arial,helvetica,sans-serif; font-size: 11px; font-weight: bold }
  A.tab:visited { text-decoration: none; color: #000000; font-family: verdana,arial,helvetica,sans-serif; font-size: 11px; font-weight: bold }
  A.tab:hover { text-decoration: underline; color: #000000; font-family: verdana,arial,helvetica,sans-serif; font-size: 11px; font-weight: bold }
  A.path:link { text-decoration: none; color: #0000CC; font-family: verdana,arial,helvetica,sans-serif; font-size: 14px; font-weight: bold }
  A.path:visited { text-decoration: none; color: #0000CC; font-family: verdana,arial,helvetica,sans-serif; font-size: 14px; font-weight: bold }
  A.path:hover { text-decoration: underline; color: #0000CC; font-family: verdana,arial,helvetica,sans-serif; font-size: 14px; font-weight: bold }
  A.remote:link { text-decoration: none; color: #0000CC; font-family: verdana,arial,helvetica,sans-serif; font-size: 10px; font-weight: bold }
  A.remote:visited { text-decoration: none; color: #0000CC; font-family: verdana,arial,helvetica,sans-serif; font-size: 10px; font-weight: bold }
  A.remote:hover { text-decoration: underline; color: #0000CC; font-family: verdana,arial,helvetica,sans-serif; font-size: 10px; font-weight: bold }
  A.file:link { text-decoration: none; color: #0000CC; font-family: verdana,arial,helvetica,sans-serif; font-size: 12px; font-weight: bold }
  A.file:visited { text-decoration: none; color: #0000CC; font-family: verdana,arial,helvetica,sans-serif; font-size: 12px; font-weight: bold }
  A.file:hover { text-decoration: underline; color: #0000CC; font-family: verdana,arial,helvetica,sans-serif; font-size: 12px; font-weight: bold }
  A.bigger:link, A.bigger:visited { text-decoration: none color: #0000FF; font-family: verdana,arial,helvetica,sans-serif; font-size: 14px }
  A.fixed:link { text-decoration: none; color: #0000CC; font-family: monospace,verdana,arial,helvetica,sans-serif; font-size: 12px; font-weight: normal }
  A.fixed:visited { text-decoration: none; color: #0000CC; font-family: monospace,verdana,arial,helvetica,sans-serif; font-size: 12px; font-weight: normal }
  A.fixed:hover { text-decoration: underline; color: #0000CC; font-family: monospace,verdana,arial,helvetica,sans-serif; font-size: 12px; font-weight: normal }
  TD { font-family: verdana,arial,helvetica,sans-serif; font-size: 12px; font-weight: normal; color: #000000 }
  TH { font-family: verdana,arial,helvetica,sans-serif; font-size: 11px; font-weight: bold; color: #000000 }
  TD.recent_activity { font-family: verdana,arial,helvetica,sans-serif; font-size: 11px; padding-left: 3px; padding-right: 3px; padding-top: 3px; padding-bottom: 3px }
  UL.none { list-style-type: none }
  DT { font-family: verdana,arial,helvetica,sans-serif; font-size: 12px; font-weight: bold; color: #000000 }
  DD { font-family: verdana,arial,helvetica,sans-serif; font-size: 12px; font-weight: normal; color: #000000 }
  table.fHeader { border: 0px; width: 100%; background: #115577 url(/headerBkgrndIcon?ac=20) repeat-x top left;}
  .logo {padding: 4px 15px 7px 7px; width: 114px;}
  .tabsspr { background: #c4c3c3 url(/tabBackgroundIcon?ac=20) repeat-x top left; border-left: 1px solid #eeeeee; padding: 2px 0px 2px 0px; }
  .tabs { background: #c4c3c3 url(/tabBackgroundIcon?ac=20) repeat-x top left; border-right: 1px solid #7f7f7f; border-left: 1px solid #eeeeee; padding: 2px 0px 2px 0px; }
  td.tabs:hover,
  .actab { background: #eeeeee url(/activeTabBackgroundIcon?ac=20) repeat-x top left; border-right: 1px solid #7f7f7f; border-left: 1px solid #eeeeee; padding: 2px 0px 2px 0px; }
  .tabs a,
  .actab a { padding: 1px 10px 1px 2px; }
  .fSmall { font-family: verdana,arial,helvetica,sans-serif; font-size: 12px }
  .fSmaller { font-family: verdana,arial,helvetica,sans-serif; font-size: 10px }
  .fNormal { font-family: verdana,arial,helvetica,sans-serif; font-size: 14px }
  .doublespace { font-family: verdana,arial,helvetica,sans-serif; font-size: 14px; font-weight: normal; line-height: 180% }
  .statusLabel { font-family: verdana,arial,helvetica,sans-serif; font-size: 10px; font-weight: bold; color: #FFCC66; padding: 4px 5px 0px 0px;}
  .statusPath { font-family: verdana,arial,helvetica,sans-serif; font-size: 10px; font-weight: bold; color: #FFCC66; padding: 0px 5px 0px 0px; }
  .pathField,
  .connectionField,
  .status { font-family: verdana,arial,helvetica,sans-serif; font-size: 11px; color: #FFFFFF }
  .pathField { padding: 0px 0px 5px 0px; white-space: nowrap; }
  .connectionField { padding: 4px 60px 0px 0px;}
  .tab { font-family: verdana,arial,helvetica,sans-serif; color: #000000; font-size: 11px; font-weight: bold }
  .version { font-family: verdana,arial,helvetica,sans-serif; font-size: 10px; color: #999999 }
  .remote { font-family: verdana,arial,helvetica,sans-serif; font-size: 10px; color: #996d02 }
  .title { font-family: verdana,arial,helvetica,sans-serif; font-size: 18px; font-weight: bold; color: #000000 }
  .copyright { font-family: verdana,arial,helvetica,sans-serif; font-size: 11px; font-style: italic; color: #999999 }
  .path { font-family: verdana,arial,helvetica,sans-serif; font-size: 14px; font-weight: bold; vertical-align: top }
  .label { font-family: verdana,arial,helvetica,sans-serif; font-size: 12px; font-weight: bold; color: #000000 }
  .normal { font-family: verdana,arial,helvetica,sans-serif; font-size: 12px; font-weight: normal; color: #000000 }
  .fixed { font-family: monospace,verdana,arial,helvetica,sans-serif; font-size: 12px; font-weight: normal; color: #000000 }
  .subtitle { font-family: verdana,arial,helvetica,sans-serif; font-size: 14px; font-weight: bold; color: #000000; text-align: left }
  .filestat { font-family: verdana,arial,helvetica,sans-serif; font-size: 10px; color: #999999; }
  .filestat A:link,
  .filestat A:visited,
  .filestat A:hover { text-decoration: none; }
  .pre { text-decoration: none; font-family: monospace; font-size: 12px; font-weight: normal; color: #000000 }
  .pre A:link { text-decoration: none; font-family: monospace; font-size: 12px; font-weight: normal; color: #000000 }
  .pre A:visited { text-decoration: none; font-family: monospace; font-size: 12px; font-weight: normal; color: #000000 }
  .pre A:hover { text-decoration: none; font-family: monospace; font-size: 12px; font-weight: normal; color: #000000 }
  TR.b td {margin: 0; border-style: solid; border-width: 0px; border-bottom: 1px solid #E8EAEC;}
  .ttip { border-style: solid; border-color: #000000; border-width: 1px; background-color: #FFFFF0; padding: 5; position: absolute; font-family: arial,sans-serif; font-size: 12px; }
  .mu { border-style: solid; border-color: #C0C0C0; border-width: 2px; background-color: #E6E6E6; padding: 0; position: absolute; }
  .mu div:hover { background-color: #008; }
  .mu div:hover a { text-decoration: none; color: #ffffff }
  .mu div {padding: 1 0 1 0;}
  .mu div div {padding: 0;}
  .muaro { background-image: url(/menuarrowIcon?ac=20); background-repeat: no-repeat; background-position: center left; }
  .muaro A:hover { background-image: url(/menuarrowhoverIcon?ac=20); background-repeat: no-repeat; background-position: center left; text-decoration: none; }
  TD.thumbnail_pane { border-style: solid; border-width: 1px; border-color: #7F7F7F }
  TR.list_row td { padding: 1px 0px; }
  TR.pathbr_row td { padding: 2px 0px;}
  div.alt_row,
  TR.alt_row { background: #f1f5fa;}
  div.menu { position: relative; cursor: default}
  div.menu ul { visibility: hidden; position: absolute; top: -20px; left: 90%; margin: 0; padding: 0 }
  div.menu:hover > ul { visibility: visible}
  .menu ul li { display: list-item; list-style: url(/notselectedIcon?ac=20) none inside; padding: 0.2em 4 0 4; white-space: nowrap}
  .menu ul li:hover { background-color: #008}
  .menu ul li.divider { list-style: none outside; border-color: #7F7F7F; border-width: 1px 0px 0px 0px; border-style: solid; padding: 0 0 0 0; margin: 2px 0 0 0; line-height: 1px; height: 1px; }
  .menu ul li.checkmark { list-style: url(/checkmarkIcon?ac=20) disc inside; }
  .menu ul li.checkmark:hover { list-style: url(/checkmarkhighlightedIcon?ac=20) disc inside; }
  .menu ul li.bullet { list-style: url(/bulletIcon?ac=20) disc inside; }
  .menu ul li.bullet:hover { list-style: url(/bullethighlightedIcon?ac=20) disc inside; }
  .menu ul li a:link,
  .menu ul li a:visited { text-decoration: none; color: #0000CC; }
  .menu ul li:hover a:link,
  .menu ul li:hover a:visited { text-decoration: none; color: #ffffff; }
  table.rev_history { width: 100%; border: 1px solid #7F7F7F; background: #ffffff;}
  table.rev_history th {padding: 2px 8px; background: #EEEEEE; text-align: left;}
  table.rev_history td {margin: 0; padding: 3px 8px; vertical-align: top;}
  table.rev_history td.top_line {margin: 0; border-top: 1px solid #d1d5da; padding: 3px 8px; vertical-align: top;}
  table.rev_history tr.top_row td {border-top: 1px solid #7F7F7F;}
  table.rev_history tr.alt_row {background: #f1f5fa;}
  div#showhideBlock { display: block; }
  div#hideBlockIcon { display: inline; }
  div#showBlockIcon { display: none; }
  div#showhideInline1 { display: inline; }
  div#showhideInline2 { display: inline; }
  div#showhideInline3 { display: inline; }
  div#showhideInline4 { display: none; }
  div#showhideBlock9 { display: block; }
  div#hideBlockIcon9 { display: inline; }
  div#showBlockIcon9 { display: none; }
  --></Style>
  <Base href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/">
<link rel="shortcut icon" href="/favicon.ico" type="image/x-icon">
<meta name="viewport" content="width=760, initial-scale=0.4, minimum-scale=0.4" />
</Head>
<Body bgcolor="#ffffff" leftmargin="0" topmargin="0" marginwidth="0" marginheight="0" onClick="hideMenu()">
<!-- BEGIN INFO PANE -->
<Table Border="0" Width="100%" Cellpadding="0" Cellspacing="1">
<tr>
<td>
<Table Border="0" Width="100%" Cellpadding="0" Cellspacing="1" Bgcolor="#7F7F7F">
<tr>
<td colspan="5">
<Table Border="0" Width="100%" Cellpadding="0" Cellspacing="0" class="fHeader">
<tr>
<td valign="middle" rowspan="2" class="logo">
<img src="/logoviewerIcon?ac=20" height="46" width="114" border="0" alt="" title="" vspace="0"></td>
<td class="statusLabel">
Server:
</td>
<td width="100%">
<Table Border="0" Cellspacing="0">
<tr>
<td class="connectionField">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//?ac=151" class="status">perforce.freebsd.org:1666
</a>
</td>
<td class="statusLabel">
Client:
<td class="connectionField">
realcgi
</td>
</tr>
</Table>
</td>
</tr>
<tr>
<td class="statusPath">
Path:
</td>
<Form method="POST" enctype="application/x-www-form-urlencoded" action="ext4_subr.c?ac=152">
<td class="pathField">
<Input type=text name="goField" value="//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c" size="92">
&nbsp;
<Input type=submit name="Go" value="Go">
</td>
</Form>
</tr>
</Table>
<!-- END INFO PANE -->
<!-- BEGIN MENU BAR PANE -->
<script language="JavaScript1.2">
function go( selId ) {
location = selId.options[selId.selectedIndex].value
}

function setCheckedValue(radioObj, newValue) {
 if(!radioObj)
	return;
 var radioLength = radioObj.length;
 if(radioLength == undefined) {
	radioObj.checked = (radioObj.value == newValue.toString());
	return;
 }
 for(var i = 0; i < radioLength; i++) {
	radioObj[i].checked = false;
	if(radioObj[i].value == newValue.toString()) {
		radioObj[i].checked = true;
	}
 }
}
</script>
<!-- BEGIN TABS -->
<Table Border="0" Width="100%" Cellpadding="0" Cellspacing="0">
<tr>
<td>
<Table Border="0" Width="100%" Cellpadding="0" Cellspacing="0">
<tr bgcolor="#C4C3C3">
<td>
<Form>
</td>
<td bgcolor="#EEEEEE" nowrap class="actab">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22" class="tab"><img src="/fileIcon?ac=20" height="25" width="25" border="0" alt="" title="" align="absmiddle">Files</a>&nbsp;&nbsp;</td>
<td nowrap class="tabs">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=69&mx=50" class="tab"><img src="/submittedChangelistIcon?ac=20" height="25" width="25" border="0" alt="" title="" align="absmiddle">Submitted</a>&nbsp;&nbsp;</td>
<td nowrap class="tabs">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//?ac=82" class="tab"><img src="/branchesIcon?ac=20" height="25" width="25" border="0" alt="" title="" align="absmiddle">Branches</a>&nbsp;&nbsp;</td>
<td nowrap class="tabs">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=77" class="tab"><img src="/labelIcon?ac=20" height="25" width="25" border="0" alt="" title="" align="absmiddle">Labels</a>&nbsp;&nbsp;</td>
<td nowrap class="tabs">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//?ac=80" class="tab"><img src="/clientIcon?ac=20" height="25" width="25" border="0" alt="" title="" align="absmiddle">Clients</a>&nbsp;&nbsp;</td>
<td nowrap class="tabs">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//?ac=81" class="tab"><img src="/userIcon?ac=20" height="25" width="25" border="0" alt="" title="" align="absmiddle">Users</a>&nbsp;&nbsp;</td>
<td nowrap class="tabs">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=107&mx=25" class="tab"><img src="/jobIcon?ac=20" height="25" width="25" border="0" alt="" title="" align="absmiddle">Jobs</a>&nbsp;&nbsp;</td>
<td nowrap class="tabs">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//?ac=79" class="tab"><img src="/settingsIcon?ac=20" height="25" width="25" border="0" alt="" title="" align="absmiddle">Settings</a>&nbsp;&nbsp;</td>
<td>
</Form>
</td>
</tr>
</Table>
</td>
<td width="100%">
<Table Width="100%" Cellpadding="0" Cellspacing="0">
<tr bgcolor="#C4C3C3">
<td width="100%" nowrap class="tabsspr">
<img src="/clearpixelIcon?ac=20" height="25" width="100%" border="0" alt="" title=""></td>
</tr>
</Table>
</td>
</tr>
<!-- END TABS -->
<!-- BEGIN SUBNAVIGATION -->
<tr bgcolor="#EEEEEE">
<td>
<Table Border="0" Cellpadding="0" Cellspacing="0">
<tr>
<td width="10">
<img src="/clearpixelIcon?ac=20" height="30" width="10" border="0" alt="" title=""></td>

<script language=javascript>
</script>
<noscript>
<td>
<Form method="POST" enctype="application/x-www-form-urlencoded" action="ext4_subr.c?ac=84">
</td>
</noscript>
<td width="1">

<script language=javascript>
</script>
<noscript>
<Select name="viewValue" size ="1" onChange="go( this )">
</noscript>

<script language=javascript>
document.write("<a href='javascript:showMenu(\"ext4_subr.c\",\"a1\",\"file\",-1,\"\")' id='id_a1' title ='Menu'><nobr>Actions<img src='/clearpixelIcon?ac=20' height='25' width='2' border='0' alt='' align=absmiddle title=''><img src='/menuarrowtoolbarIcon?ac=20' height='25' width='15' border='0' alt='' align=absmiddle title=''></nobr></a></td><td valign='middle' align='right' width='3'><img src='/grayPixelIcon?ac=20' height='18' width='1' border='0' alt='' title='' vspace='0' hspace='0'>")
</script>
<noscript>
<Option selected value="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22">
Revision history
<Option value="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c">
Open head revision in browser
<Option value="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=64">
View head revision text
<Option value="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=142">
View annotated file text
<Option value="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=205">
Diff two depot files...
</Select>
</td>
<td width="3">
<img src="/clearpixelIcon?ac=20" height="30" width="3" border="0" alt="" title=""></td>
<td width="40">
<Input type=submit name="view" value="Go">
</td>
<td>
</Form>
</td>
</noscript>
<td valign="middle" align="center" width="2">
<img src="/clearpixelIcon?ac=20" height="1" width="2" border="0" alt="" title="" vspace="0" hspace="0"></a>
</td>
<td width="25">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/?ac=83"><img src="/gotreeIcon?ac=20" height="25" width="25" border="0" alt="Display tree view" title="Display tree view"></a>
</td>
<td valign="middle" align="center" width="7">
<img src="/grayPixelIcon?ac=20" height="18" width="1" border="0" alt="" title="" vspace="0" hspace="0"><img src="/clearpixelIcon?ac=20" height="1" width="2" border="0" alt="" title="" vspace="0" hspace="0"></td>
<td width="25">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22"><img src="/list_icon_onIcon?ac=20" height="25" width="25" border="0" alt="Display revision history" title="Display revision history"></a></td>
<td width="25">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&thv=d&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22"><img src="/details_icon_offIcon?ac=20" height="25" width="25" border="0" alt="Display revision history" title="Display revision history"></a></td>
<td width="25">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22"><img src="/thumbnails_icon_offIcon?ac=20" height="25" width="25" border="0" alt="Display revision history thumbnails" title="Display revision history thumbnails"></a></td>
<td width="13">
<script language=javascript>
document.write("<a href='javascript:showMenu(\"\",\"toolbarthumbs\",\"Thumbs\",-1,\"\")' id='id_toolbarthumbs'><img src='/menuarrowtoolbarIcon?ac=20' height='25' width='15' border='0' alt='' title='Thumbnail options'></a>")
</script>
<noscript>
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//?ac=79"><img src="/menuarrowtoolbarIcon?ac=20" height="25" width="15" border="0" alt="" title=""></a>
</noscript>

<div id="menu_Thumbs" class="mu" style="display:none"><Table Width="10">
<tr>
<td nowrap>
<div id="id_muThumbs1" style="display:none" onMouseOver="showhi('id_muThumbs1')" onMouseOut="hidehi('id_muThumbs1')"><a href="javascript:runurl('/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22')"><img src="bulletIcon?ac=20" height="12" width="8" border="0" alt="" title="" align="top" id="id_muThumbs1p"><img src="bullethighlightedIcon?ac=20" height="12" width="8" border="0" alt="" title="" align="top" id="id_muThumbs1h"><nobr> Large 160x160</nobr></a><br>
</div>
<div id="id_muThumbs2" style="display:none"><a href="javascript:runurl('/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&thz=m&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22')"><img src="/clearpixelIcon?ac=20" height="12" width="8" border="0" alt="" title="" align="top"><nobr> Medium 120x120</nobr></a><br>
</div>
<div id="id_muThumbs4" style="display:none"><a href="javascript:runurl('/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&thz=s&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22')"><img src="/clearpixelIcon?ac=20" height="12" width="8" border="0" alt="" title="" align="top"><nobr> Small 80x80</nobr></a><br>
</div>
<div id="id_muThumbs8" style="display:none"><img src="/grayPixelIcon?ac=20" height="1" width="100%" border="0" alt="" title="" vspace="2" hspace="0"><br>
</div>
<div id="menuThumbs" class="menu">
<div id="id_muThumbs16" style="display:none"><a href="javascript:runurl('/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//?ac=79')"><img src="/clearpixelIcon?ac=20" height="12" width="8" border="0" alt="" title="" align="top"><nobr> Columns &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &gt;&gt;</a></nobr></a><br>
</div>
<ul>
<Table Width="30%" Cellpadding="2" Bgcolor="#c0c0c0">
<tr bgcolor="#e6e6e6">
<td>
  <li><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&thc=1&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22">&nbsp;&nbsp;1&nbsp;&nbsp;&nbsp;&nbsp;</a></li>
  <li><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&thc=2&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22">&nbsp;&nbsp;2&nbsp;&nbsp;&nbsp;&nbsp;</a></li>
  <li><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&thc=3&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22">&nbsp;&nbsp;3&nbsp;&nbsp;&nbsp;&nbsp;</a></li>
  <li class="bullet"><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22">&nbsp;&nbsp;4&nbsp;&nbsp;&nbsp;&nbsp;</a></li>
  <li><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&thc=5&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22">&nbsp;&nbsp;5&nbsp;&nbsp;&nbsp;&nbsp;</a></li>
  <li><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&thc=6&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22">&nbsp;&nbsp;6&nbsp;&nbsp;&nbsp;&nbsp;</a></li>
  <li><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&thc=7&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22">&nbsp;&nbsp;7&nbsp;&nbsp;&nbsp;&nbsp;</a></li>
  <li><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&thc=8&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22">&nbsp;&nbsp;8&nbsp;&nbsp;&nbsp;&nbsp;</a></li>
  <li><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&thc=9&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22">&nbsp;&nbsp;9&nbsp;&nbsp;&nbsp;&nbsp;</a></li>
  <li><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&thc=10&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22">&nbsp;10&nbsp;&nbsp;&nbsp;&nbsp;</a></li>
</td>
</tr>
</Table>
  </ul>
</div><div id="id_muThumbs32" style="display:none"><a href="javascript:runurl('/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&thw=y&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22')"><img src="/clearpixelIcon?ac=20" height="12" width="8" border="0" alt="" title="" align="top"><nobr> Autowrap Thumbnails&nbsp;</nobr></a><br>
</div>
<div id="id_muThumbs64" style="display:none"><img src="/grayPixelIcon?ac=20" height="1" width="100%" border="0" alt="" title="" vspace="2" hspace="0"><br>
</div>
<div id="id_muThumbs128" style="display:none"><a href="javascript:runurl('/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&thm=y&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22')"><img src="/clearpixelIcon?ac=20" height="12" width="8" border="0" alt="" title="" align="top"><nobr> Magnify Small Images&nbsp;</nobr></a><br>
</div>
<div id="id_muThumbs256" style="display:none"><a href="javascript:runurl('/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&rt=s&thb=y&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22')"><img src="/clearpixelIcon?ac=20" height="12" width="8" border="0" alt="" title="" align="top"><nobr> Show Borders</nobr></a><br>
</div>
</td>
</tr>
</Table>
</div></td>
<td valign="middle" align="center" width="2">
<img src="/clearpixelIcon?ac=20" height="1" width="2" border="0" alt="" title="" vspace="0" hspace="0"></a>
</td>
<td width="25">
<a href="ext4_subr.c?ac=22&fl=-i"><img src="/branchHistOffIcon?ac=20" height="25" width="25" border="0" alt="Show branching history" title="Show branching history"></a>
</td>
<td valign="middle" align="center" width="7">
<img src="/clearpixelIcon?ac=20" height="1" width="2" border="0" alt="" title="" vspace="0" hspace="0"><img src="/grayPixelIcon?ac=20" height="18" width="1" border="0" alt="" title="" vspace="0" hspace="0"></td>
<td valign="middle" align="center" width="2">
<img src="/clearpixelIcon?ac=20" height="1" width="2" border="0" alt="" title="" vspace="0" hspace="0"></a>
</td>
<td width="25">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=64"><img src="/showtextIcon?ac=20" height="25" width="25" border="0" alt="View head revision text" title="View head revision text"></a>
</td>
<td valign="middle" align="center" width="7">
<img src="/clearpixelIcon?ac=20" height="1" width="4" border="0" alt="" title="" vspace="0" hspace="0"><img src="/grayPixelIcon?ac=20" height="18" width="1" border="0" alt="" title="" vspace="0" hspace="0"></a>
</td>
<td valign="middle" align="center" width="3">
<img src="/clearpixelIcon?ac=20" height="1" width="3" border="0" alt="" title="" vspace="0" hspace="0"></a>
</td>
<td align="right">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//?ac=144&lac=22"><img src="/cancelIcon?ac=20" height="25" width="25" border="0" alt="Cancel operation" title="Cancel operation"></a></td>
<td valign="middle" align="center" width="4">
<img src="/clearpixelIcon?ac=20" height="1" width="4" border="0" alt="" title="" vspace="0" hspace="0"></a>
</td>
<td valign="middle" align="center" width="6">
<img src="/grayPixelIcon?ac=20" height="18" width="1" border="0" alt="" title="" vspace="0" hspace="0"></a>
</td>
<td width="25">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22"><img src="/recentactivityOnIcon?ac=20" height="25" width="25" border="0" alt="Hide recent activity bar" title="Hide recent activity bar"></a>
</td>
<td valign="middle" align="center" width="2">
<img src="/clearpixelIcon?ac=20" height="1" width="2" border="0" alt="" title="" vspace="0" hspace="0"></a>
</td>
<td width="25">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22"><img src="/goOnIcon?ac=20" height="25" width="25" border="0" alt="Hide go to bar" title="Hide go to bar"></a>
</td>
<td valign="middle" align="center" width="11">
<img src="/clearpixelIcon?ac=20" height="1" width="5" border="0" alt="" title="" vspace="0" hspace="0"><img src="/grayPixelIcon?ac=20" height="18" width="1" border="0" alt="" title="" vspace="0" hspace="0"></a>
</td>
<td width="25">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//?ac=151"><img src="/infoIcon?ac=20" height="25" width="25" border="0" alt="Information" title="Information"></a>
</td>
<td valign="middle" align="center" width="2">
<img src="/clearpixelIcon?ac=20" height="1" width="2" border="0" alt="" title="" vspace="0" hspace="0"></a>
</td>
<td width="30">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b@//filebrowser?ac=21" class="nav_link"><img src="/helpIcon?ac=20" height="25" width="25" border="0" alt="Help" title="Help"></a>
</td>
</tr>
</Table>
</td>
<td nowrap>
</td>
</tr>
</Table>
<div id="menu_revhist" class="mu" style="display:none">
<Table Width="10">
<tr>
<td nowrap>

<div id="id_murevhist1" style="display:none"><a href="javascript:runcmd(98)"><img src="/clearpixelIcon?ac=20" height="17" width="21" border="0" alt="" title="" align="top"><nobr>Open Revision in Browser&nbsp;</nobr></a>
<br>
</div><div id="id_murevhist2" style="display:none" onMouseOver="showhi('id_murevhist2')" onMouseOut="hidehi('id_murevhist2')"><a href="javascript:runcmd(64)"><img src="/showtext17Icon?ac=20" height="17" width="21" border="0" alt="" title="" align="top" id="id_murevhist2p"><img src="/showtext17highlightedIcon?ac=20" height="17" width="21" border="0" alt="" title="" align="top" id="id_murevhist2h"><nobr>View Revision Text</nobr></a>
<br>
</div><div id="id_murevhist4" style="display:none"><img src="/grayPixelIcon?ac=20" height="1" width="100%" border="0" alt="" title="" vspace="2" hspace="0"><br>
</div><div id="id_murevhist8" style="display:none" onMouseOver="showhi('id_murevhist8')" onMouseOut="hidehi('id_murevhist8')"><a href="javascript:runcmd(185)"><img src="/rundiffprev17Icon?ac=20" height="17" width="21" border="0" alt="" title="" align="top" id="id_murevhist8p"><img src="/rundiffprev17highlightedIcon?ac=20" height="17" width="21" border="0" alt="" title="" align="top" id="id_murevhist8h"><nobr>Diff vs. Previous Revision</nobr></a>
<br>
</div><div id="id_murevhist16" style="display:none"><a href="javascript:if (validateFilelogForm()) document.filelogForm.submit();"><img src="/clearpixelIcon?ac=20" height="17" width="21" border="0" alt="" title="" align="top"><nobr>Diff vs. Selected Revision</nobr></a>
<br>
</div></td>
</tr>
</Table>
</div>
</td>
</tr>
<!-- END SUBNAVIGATION -->
<!-- END MENU BAR PANE -->
<!-- BEGIN GOTO PANE -->
<tr bgcolor="#EEEEEE">
<td colspan="5">
<Table Width="100%" Cellpadding="1" Cellspacing="5" Bgcolor="#EEEEEE">
<tr bgcolor="#EEEEEE">
<td>
<Form method="POST" enctype="application/x-www-form-urlencoded" action="ext4_subr.c?ac=204">
</td>
<td nowrap>
Go to:
<Select name="type">
<Option value=S>
Changelist
<Option selected value=B>
Branch
<Option value=L>
Label
<Option value=C>
Client
<Option value=U>
User
<Option value=J>
Job
</Select>
<Input type=text name="p4" value="" size="60">
<Input type=submit name="p4go" value="Go">
</td>
<td>
</Form>
</td>
</tr>
</td>
</tr>
</Table>
<!-- END GOTO PANE -->
<!-- BEGIN UPDATE PANE -->
<tr bgcolor="#FDE8B0">
<td valign="top" class="recent_activity">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@/183855?ac=10" class="nav_link">183855</a>&nbsp;2010/09/16 <span style="color: #A10000; font-weight: bold">lz</span>        Rename file prefix name </span></td>
<td valign="top" class="recent_activity">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@/183858?ac=10" class="nav_link">183858</a>&nbsp;2010/09/16 <span style="color: #A10000; font-weight: bold">lz</span>        Make ext4fs can be compi</span></td>
</tr>
<!-- END UPDATE PANE -->
</Table>
</td>
</tr>
<tr>
<td>
<Table Border="0" Width="100%" Cellpadding="0" Cellspacing="0">
<tr>
<td width="10">
<img src="/clearpixelIcon?ac=20" height="1" width="10" border="0" alt="" title=""></td>
<td width="100%">
<Table Border="0" Width="100%" Cellpadding="0" Cellspacing="0">
<tr>
<td>
<img src="/clearpixelIcon?ac=20" height="5" width="0" border="0" alt="" title=""></td>
</tr>
<!-- BEGIN FILEBROWSER PANE -->
<tr>
<td colspan="3">
<Table>
<tr>
<td>
<span class="title">Revision History: &nbsp;</span></td>
</tr>
</Table>
</td>
</tr>
<tr>
<td valign="top" colspan="3">
<img src="/grayPixelIcon?ac=20" height="1" width="100%" border="0" alt="" title="" vspace="0" hspace="0"></td>
</tr>
<tr>
<td width="100%">
<Table Width="100%" Cellpadding="0" Cellspacing="0">
<tr>
<td>
<img src="/clearpixelIcon?ac=20" height="5" width="5" border="0" alt="" title=""><div id="menu_file" class="mu" style="display:none">
<Table Width="10">
<tr>
<td nowrap>

<div id="id_mufile1" style="display:none" onMouseOver="showhi('id_mufile1')" onMouseOut="hidehi('id_mufile1')"><a href="javascript:runcmd(22)"><img src="/showfilelog17Icon?ac=20" height="17" width="21" border="0" alt="" title="" align="top" id="id_mufile1p"><img src="/showfilelog17highlightedIcon?ac=20" height="17" width="21" border="0" alt="" title="" align="top" id="id_mufile1h"><nobr>Revision History</nobr></a><br>
</div><div id="id_mufile5" style="display:none"><img src="/grayPixelIcon?ac=20" height="1" width="100%" border="0" alt="" title="" vspace="2" hspace="0"><br>
</div><div id="id_mufile4" style="display:none"><a href="javascript:runcmd(98)"><img src="/clearpixelIcon?ac=20" height="17" width="21" border="0" alt="" title="" align="top"><nobr>Open Head Rev in Browser</nobr></a><br>
</div><div id="id_mufile17" style="display:none"><img src="/grayPixelIcon?ac=20" height="1" width="100%" border="0" alt="" title="" vspace="2" hspace="0"><br>
</div><div id="id_mufile68" style="display:none"><img src="/grayPixelIcon?ac=20" height="1" width="100%" border="0" alt="" title="" vspace="2" hspace="0"><br>
</div><div id="id_mufile1025" style="display:none"><img src="/grayPixelIcon?ac=20" height="1" width="100%" border="0" alt="" title="" vspace="2" hspace="0"><br>
</div><div id="id_mufile512" style="display:none" onMouseOver="showhi('id_mufile512')" onMouseOut="hidehi('id_mufile512')"><a href="javascript:runcmd(64)"><img src="/showtext17Icon?ac=20" height="17" width="21" border="0" alt="" title="" align="top" id="id_mufile512p"><img src="/showtext17highlightedIcon?ac=20" height="17" width="21" border="0" alt="" title="" align="top" id="id_mufile512h"><nobr>View Head Revision Text</nobr></a><br>
</div><div id="id_mufile513" style="display:none"><a href="javascript:runcmd(203)"><img src="/clearpixelIcon?ac=20" height="17" width="21" border="0" alt="" title="" align="top"><nobr>View Annotated File Text</nobr></a><br>
</div><div id="id_mufile6" style="display:none"><img src="/grayPixelIcon?ac=20" height="1" width="100%" border="0" alt="" title="" vspace="2" hspace="0"><br>
<a href="javascript:runcmd(205)"><img src="/clearpixelIcon?ac=20" height="17" width="21" border="0" alt="" title="" align="top"><nobr>Diff Two Depot Files... &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;</nobr></a><br>
</div><div id="id_mufile262145" style="display:none"><img src="/grayPixelIcon?ac=20" height="1" width="100%" border="0" alt="" title="" vspace="2" hspace="0"><br>
</div></td>
</tr>
</Table>
</div>
<script language=javascript>
setmushow(362079);
</script>
</td>
</tr>
<tr>
<td valign="top">

<script language=javascript>
function toggleFileDetails()
{
var elem, vis, viss, vish, vis1, vis2, vis3, vis4;
if( document.getElementById )
	elem = document.getElementById( 'showhideBlock' );
else if( document.all )
	elem = document.all['showhideBlock'];
else if( document.layers )
	elem = document.layers['showhideBlock'];
vis = elem.style;
if(vis.display==''&&elem.offsetWidth!=undefined&&elem.offsetHeight!=undefined)
	vis.display = (elem.offsetWidth!=0&&elem.offsetHeight!=0)?'block':'none';
vis.display = (vis.display==''||vis.display=='block')?'none':'block';
if( document.getElementById )
	elem = document.getElementById( 'showBlockIcon' );
else if( document.all )
	elem = document.all['showBlockIcon'];
else if( document.layers )
	elem = document.layers['showBlockIcon'];
viss = elem.style;
viss.display = (vis.display==''||vis.display=='block')?'none':'inline';
if( document.getElementById )
	elem = document.getElementById( 'hideBlockIcon' );
else if( document.all )
	elem = document.all['hideBlockIcon'];
else if( document.layers )
	elem = document.layers['hideBlockIcon'];
vish = elem.style;
vish.display = (vis.display==''||vis.display=='block')?'inline':'none';
if( document.getElementById )
	elem = document.getElementById( 'showhideInline1' );
else if( document.all )
	elem = document.all['showhideInline1'];
else if( document.layers )
	elem = document.layers['showhideInline1'];
vis1 = elem.style;
vis1.display = (vis.display==''||vis.display=='block')?'inline':'none';
if( document.getElementById )
	elem = document.getElementById( 'showhideInline2' );
else if( document.all )
	elem = document.all['showhideInline2'];
else if( document.layers )
	elem = document.layers['showhideInline2'];
vis2 = elem.style;
vis2.display = (vis.display==''||vis.display=='block')?'inline':'none';
if( document.getElementById )
	elem = document.getElementById( 'showhideInline3' );
else if( document.all )
	elem = document.all['showhideInline3'];
else if( document.layers )
	elem = document.layers['showhideInline3'];
vis3 = elem.style;
vis3.display = (vis.display==''||vis.display=='block')?'inline':'none';
if( document.getElementById )
	elem = document.getElementById( 'showhideInline4' );
else if( document.all )
	elem = document.all['showhideInline4'];
else if( document.layers )
	elem = document.layers['showhideInline4'];
vis4 = elem.style;
vis4.display = (vis.display==''||vis.display=='block')?'none':'inline';
}
document.write("<a href='javascript:toggleFileDetails();' title='Show/Hide File Details'>")
document.write("<div id='showBlockIcon'>")
document.write("<img src='/plusBoxIcon?ac=20' height='20' width='20' border='0' alt='Show File Details' title='Show File Details'>")
document.write("</div>")
document.write("<div id='hideBlockIcon'>")
document.write("<img src='/minusBoxIcon?ac=20' height='20' width='20' border='0' alt='Hide File Details' title='Hide File Details'>")
document.write("</div>")
document.write("</a>")
document.write("</td><td width='100%'>")
</script>
<span class="path"><b>
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&ra=s&rg=b&c=Qui@//?ac=83" class="bigger">//</a><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&ra=s&rg=b&c=Qui@//depot/?ac=83" class="bigger">depot</a><span class="bigger">/</span><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&ra=s&rg=b&c=Qui@//depot/projects/?ac=83" class="bigger">projects</a><span class="bigger">/</span><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&ra=s&rg=b&c=Qui@//depot/projects/soc2010/?ac=83" class="bigger">soc2010</a><span class="bigger">/</span><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/?ac=83" class="bigger">ext4fs</a><span class="bigger">/</span><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/?ac=83" class="bigger">src</a><span class="bigger">/</span><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/?ac=83" class="bigger">sys</a><span class="bigger">/</span><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/?ac=83" class="bigger">fs</a><span class="bigger">/</span><a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/?ac=83" class="bigger">ext4fs</a><span class="bigger">/</span><a href="ext4_subr.c?ac=22" class="bigger">ext4_subr.c</a></b>
</span></td>
</tr>
<tr>
<td colspan="2">
<script language=javascript>
document.write("<div id='showhideBlock'>")
</script>
<Table Border="0" Width="100%" Cellpadding="4" Cellspacing="0" Bgcolor="#FFFFFF">
<tr bgcolor="#FFFFFF">
<td>
<Table Border="0" Cellpadding="2" Cellspacing="0" Bgcolor="#FFFFFF">
<tr>
<td>
<span class="label">Type:
</span></td>
<td>
<img src="/clearpixelIcon?ac=20" height="1" width="5" border="0" alt="" title=""></td>
<td>
text+ko
</td>
<td>
<img src="/clearpixelIcon?ac=20" height="1" width="30" border="0" alt="" title=""></td>
<td>
<span class="label"></span></td>
<td>
<img src="/clearpixelIcon?ac=20" height="1" width="5" border="0" alt="" title=""></td>
<td>
</td>
<td>
<img src="/clearpixelIcon?ac=20" height="1" width="30" border="0" alt="" title=""></td>
<td>
<span class="label">Head rev size:</span></td>
<td>
<img src="/clearpixelIcon?ac=20" height="1" width="5" border="0" alt="" title=""></td>
<td>
4.89&nbsp;KB</td>
</tr>
<tr>
</tr>
<tr>
<td>
<span class="label">Head:</span></td>
<td>
<img src="/clearpixelIcon?ac=20" height="1" width="5" border="0" alt="" title=""></td>
<td>
<a href="//127.0.1.3:2083/@rev1=head@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c" title="Open head revision in browser">#2</a>
</td>
<td>
<img src="/clearpixelIcon?ac=20" height="1" width="30" border="0" alt="" title=""></td>
</tr>
<tr>
<td colspan="11">
<Table Cellpadding="0" Cellspacing="0">
<tr>
<td nowrap>
</td>
</tr>
</Table>
</td>
</tr>
</Table>
</td>
</tr>
</Table>
<script language=javascript>
document.write("</div>")
</script>
</td>
</tr>
</Table>
</td>
</tr>
<tr>
<td valign="top" colspan="3" width="100%">
<div id="showhideInline1">
<img src="/grayPixelIcon?ac=20" height="1" width="100%" border="0" alt="" title="" vspace="0" hspace="0"></div>
</td>
</tr>
</tr>
</Table>
</td>
<td width="10">
<img src="/clearpixelIcon?ac=20" height="1" width="10" border="0" alt="" title=""></td>
</tr>
</Table>
</td>
</tr>
</Table>
</td>
</tr>
<!-- END FILEBROWSER PANE -->
<!-- BEGIN FILELOG PANE -->
<Table Border="0" Width="100%" Cellpadding="0" Cellspacing="1">
<tr>
<td width="8">
<img src="/clearpixelIcon?ac=20" height="1" width="8" border="0" alt="" title=""></td>
<td>
<Table Border="0" Width="100%" Cellpadding="0" Cellspacing="1">
<tr>
<td>
<img src="/clearpixelIcon?ac=20" height="10" width="1" border="0" alt="" title=""></td>
</tr>
<tr>
<td>
<Form method="POST" enctype="application/x-www-form-urlencoded" action="ext4_subr.c?ac=191" name="filelogForm">
<Input type=hidden name="rev2">
<Table Cellspacing="0" class="rev_history">
<tr>
<div id="diff2Help" class="ttip" style="display:none"><nobr>To diff 2 revisions:<br>
&nbsp; 1) Select radio button for first revision<br>
&nbsp; 2) Choose &quot;Diff vs. Selected Revision&quot; from the&nbsp;<br>
&nbsp; &nbsp; &nbsp; drop down menu of the second revision.</nobr></div><script language=javascript>
document.write("<th align='center' style='padding:0 0'><center><div id=\"diff2Icon\"><a onMouseOver='showDiv(\"diff2Help\", \"diff2Icon\")' onMouseOut='hideDiv(\"diff2Help\")'><img src='/rundiff17Icon?ac=20' height='17' width='21' border='0' alt='' title=''></div></a></center></th><th style='padding:0 0'><img src='/clearpixelIcon?ac=20' height='1' width='1' border='0' alt='' title=''></th>")</script>
<th align="left">
Rev<img src="/clearpixelIcon?ac=20" height="0" width="78" border="0" alt="" title=""></th>
<th align="left">
Changelist</th>
<th align="left">
Date</th>
<th align="left">
User</th>
<th align="left">
Type</th>
<th align="left" width="60%">
Changelist&nbsp;Description/Action</th>
</tr>
<tr valign="top" class="top_row">
<script language=javascript>
document.write("<td rowspan='2'>")
document.write("<Input type=radio name='revs' value='2' id='2' title='Select #2'></td><td rowspan='2' bgcolor='#dfe3e8' style='padding:0 0'></td>")
</script>
<td rowspan="1">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&sr=183858&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c" title="Open revision in browser"><b>&nbsp;2&nbsp;</b></a><script language=javascript>
document.write("<span class='muaro'><a title='Menu' href='javascript:showMenu(\"ext4_subr.c\",\"0_2\",\"revhist\",-1,\"&rev1=2\")' onClick='document.forms.filelogForm.rev2.value=\"2\";' id='id_0_2'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</a></span>")
</script>
</td>
<td rowspan="2">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@/183858?ac=10" title="View changelist">183858</a></td>
<td rowspan="2">
2010/09/16</td>
<td rowspan="2">
lz</td>
<td rowspan="2">
text+ko</td>
<td>
       Make ext4fs can be compiled.<br></td>
</tr>
<tr>
<td valign="bottom">
<nobr><img src="/clearpixelIcon?ac=20" height="1" width="28" border="0" alt="" title=""><a href="ext4_subr.c?ac=64&rev1=2" title="View revision text"><img src="/showtextsmallIcon?ac=20" height="18" width="25" border="0" alt="View revision text" title="View revision text"></a>
<a href="ext4_subr.c?ac=19&rev1=1&rev2=2"><img src="/rundiffprevsmallIcon?ac=20" height="18" width="25" border="0" alt="Diff rev #1 vs. rev #2" title="Diff rev #1 vs. rev #2"></a></nobr>
</td>
<td nowrap class="top_line">
edit</td>
</tr>
<tr valign="top" class="alt_row">
<script language=javascript>
document.write("<td rowspan='2' class='top_line'>")
document.write("<Input type=radio name='revs' value='1' id='1' title='Select #1'></td><td rowspan='2' bgcolor='#dfe3e8' style='padding:0 0'></td>")
</script>
<td rowspan="1" class="top_line">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&sr=183855&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c" title="Open revision in browser"><b>&nbsp;1&nbsp;</b></a><script language=javascript>
document.write("<span class='muaro'><a title='Menu' href='javascript:showMenu(\"ext4_subr.c\",\"0_1\",\"revhist\",247,\"&rev1=1\")' onClick='document.forms.filelogForm.rev2.value=\"1\";' id='id_0_1'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</a></span>")
</script>
</td>
<td rowspan="2" class="top_line">
<a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@/183855?ac=10" title="View changelist">183855</a></td>
<td rowspan="2" class="top_line">
2010/09/16</td>
<td rowspan="2" class="top_line">
lz</td>
<td rowspan="2" class="top_line">
text+ko</td>
<td class="top_line">
       Rename file prefix name from ext2_ to ext4_.<br></td>
</tr>
<tr class="alt_row">
<td valign="bottom">
<br>
<nobr><img src="/clearpixelIcon?ac=20" height="1" width="28" border="0" alt="" title=""><a href="ext4_subr.c?ac=64&rev1=1" title="View revision text"><img src="/showtextsmallIcon?ac=20" height="18" width="25" border="0" alt="View revision text" title="View revision text"></a></nobr>
</td>
<td nowrap class="top_line">
branch<br>
branch from <a href="/@md=d&cd=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/&cdf=//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c&ra=s&rg=b&c=Qui@//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext2_subr.c?ac=22">//depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext2_subr.c</a>#1</td>
</tr>
</Table>
</Form>
</td>
</tr>
<!-- END FILELOG PANE -->
</Table>
</td>
<td width="9">
<img src="/clearpixelIcon?ac=20" height="1" width="9" border="0" alt="" title=""></td>
</tr>
</Table>
</td>
</tr>
</Table>
</Table>
</Table>
<Table Width="100%" Cellpadding="2" Cellspacing="0">
<tr>
<td>
<img src="/clearpixelIcon?ac=20" height="10" width="1" border="0" alt="" title=""></td>
</tr>
<tr>
<td width="5">
<img src="/clearpixelIcon?ac=20" height="1" width="5" border="0" alt="" title=""></td>
<td colspan="100%">
<img src="/grayPixelIcon?ac=20" height="1" width="100%" border="0" alt="" title="" vspace="0" hspace="0"></td>
<td width="5">
<img src="/clearpixelIcon?ac=20" height="1" width="5" border="0" alt="" title=""></td>
</tr>
</Table>
<Table Width="100%" Cellpadding="2" Cellspacing="0">
<tr>
<td width="5">
<img src="/clearpixelIcon?ac=20" height="1" width="5" border="0" alt="" title=""></td>
<td nowrap>
<span class="copyright">Copyright 2008 Perforce Software. All rights reserved.</span></td>
<td valign="top" align="right" class="permalink">
<a href="/depot/projects/soc2010/ext4fs/src/sys/fs/ext4fs/ext4_subr.c?ac=22">Permalink</a>
</td>
<td width="5">
<img src="/clearpixelIcon?ac=20" height="1" width="5" border="0" alt="" title=""></td>
</tr>
</Table>
</Body>
</Html>
